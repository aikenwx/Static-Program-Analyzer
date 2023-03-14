#include "sp.h"

#include <memory>
#include <queue>
#include <unordered_set>

#include "exceptions/parser_error.h"
#include "exceptions/semantic_error.h"
#include "exceptions/syntax_error.h"
#include "sp/ast/ast.h"
#include "sp/ast/program_node.h"
#include "sp/design_extractor/assign_exp_extractor.h"
#include "sp/design_extractor/ast_elem_extractor.h"
#include "sp/design_extractor/call_validator.h"
#include "sp/design_extractor/follows_extractor.h"
#include "sp/design_extractor/parent_extractor.h"
#include "sp/design_extractor/stmt_modifies_extractor.h"
#include "sp/design_extractor/stmt_uses_extractor.h"
#include "sp/parser/simple_chain_parser.h"
#include "sp/rel/assign_exp_relationship.h"
#include "sp/rel/const_relationship.h"
#include "sp/rel/follows_stmt_stmt_relationship.h"
#include "sp/rel/modifies_proc_var_relationship.h"
#include "sp/rel/modifies_stmt_var_relationship.h"
#include "sp/rel/parent_stmt_stmt_relationship.h"
#include "sp/rel/proc_relationship.h"
#include "sp/rel/stmt_relationship.h"
#include "sp/rel/uses_proc_var_relationship.h"
#include "sp/rel/uses_stmt_var_relationship.h"
#include "sp/rel/var_relationship.h"
#include "token/token.h"
#include "tokenizer/simple_tokenizer.h"
#include "util/instance_of.h"
#include "util/interval_tree.h"

namespace sp {
std::shared_ptr<ast::ProgramNode> ToProgramNode(std::shared_ptr<ast::INode> root) {
  if (!util::instance_of<ast::ProgramNode>(root)) {
    throw exceptions::SyntaxError("Invalid program");
  }

  std::shared_ptr<ast::ProgramNode> programNode =
      std::static_pointer_cast<ast::ProgramNode>(root);

  std::unordered_set<std::string> procedures;
  for (const auto& procNode : programNode->GetProcedures()) {
    if (procedures.find(procNode->GetName()) != procedures.end()) {
      throw exceptions::SemanticError("Duplicate procedure name: " +
                                      procNode->GetName());
    }
    procedures.insert(procNode->GetName());
  }

  // validate that all call statements call a procedure that exists
  auto callValidator = std::make_shared<design_extractor::CallValidator>();
  root->AcceptVisitor(root, callValidator, 0);

  return programNode;
}

template <typename E, typename R>
inline std::vector<std::shared_ptr<R>> Visit(
    std::shared_ptr<ast::ProgramNode> programNode) {
  auto extractor = std::make_shared<E>();
  programNode->AcceptVisitor(programNode, extractor, 0);
  return extractor->GetRelationships();
}

void PopulateAstEntities(
    const std::vector<std::shared_ptr<rel::Relationship>>& astElemRelationships,
    PopulateFacade* popFacade) {
  for (const auto& rel : astElemRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::CALL_STMT) {
      std::shared_ptr<rel::CallStmtRelationship> callStmtRel =
          std::static_pointer_cast<rel::CallStmtRelationship>(rel);

      popFacade->storeCallStatement(callStmtRel->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::PROC) {
      std::shared_ptr<rel::ProcRelationship> procRel =
          std::static_pointer_cast<rel::ProcRelationship>(rel);
      std::string procName = procRel->procedureName();
      std::shared_ptr<ast::ProcedureNode> procNode = procRel->procedureNode();

      popFacade->storeProcedure(procName);
    } else if (rel->relationshipType() == rel::RelationshipType::ASSIGN_STMT) {
      popFacade->storeAssignmentStatement(
          std::static_pointer_cast<rel::AssignStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::IF_STMT) {
      popFacade->storeIfStatement(
          std::static_pointer_cast<rel::IfStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::WHILE_STMT) {
      popFacade->storeWhileStatement(
          std::static_pointer_cast<rel::WhileStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::PRINT_STMT) {
      popFacade->storePrintStatement(
          std::static_pointer_cast<rel::PrintStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::READ_STMT) {
      popFacade->storeReadStatement(
          std::static_pointer_cast<rel::ReadStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::CONST) {
      popFacade->storeConstant(
          std::static_pointer_cast<rel::ConstRelationship>(rel)->value());
    } else if (rel->relationshipType() == rel::RelationshipType::VAR) {
      popFacade->storeVariable(
          std::static_pointer_cast<rel::VarRelationship>(rel)->variableName());
    }
  }
}

void ResolveCallStmtRels(
    const std::vector<std::shared_ptr<rel::Relationship>>& astElemRelationships,
    std::vector<std::shared_ptr<rel::CallStmtRelationship>>& callStmtRels) {
  for (auto& rel : astElemRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::CALL_STMT) {
      callStmtRels.push_back(
          std::static_pointer_cast<rel::CallStmtRelationship>(rel));
    }
  }
}

void ResolveProcedureInfo(
    std::shared_ptr<ast::ProgramNode> programNode,
    std::unordered_map<std::string, std::shared_ptr<ast::ProcedureNode>>&
        procedureByName,
    util::IntervalTree<int, std::string>& procedureNameByLine) {
  for (const auto& procNode : programNode->GetProcedures()) {
    procedureByName[procNode->GetName()] = procNode;

    int startStmtNum = procNode->GetStartStatementNumber();
    int endStmtNum = procNode->GetEndStatementNumber();
    procedureNameByLine.Insert({startStmtNum, endStmtNum}, procNode->GetName());
  }
}

void ResolveCalls(
    const std::vector<std::shared_ptr<rel::CallStmtRelationship>>& callStmtRels,
    std::unordered_map<std::string, std::unordered_set<std::string>>& calls) {
  // populate calls
  for (const auto& rel : callStmtRels) {
    std::string calledProcName = rel->procedureName();
    std::string callerProcName = rel->parentProcedureName();

    calls[callerProcName].emplace(calledProcName);
  }
}

void ResolveCallsStarSingleProc(
    const std::string& proc,
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        calls,
    std::unordered_map<std::string, std::unordered_set<std::string>>&
        callsStar) {
  std::unordered_set<std::string>* callsStarProcs = &callsStar[proc];
  std::unordered_set<std::string> traversed;
  std::queue<std::string> traverseQueue;

  traverseQueue.push(proc);
  while (!traverseQueue.empty()) {
    std::string calledProcName = traverseQueue.front();
    traverseQueue.pop();

    // if we've already traversed this procedure
    if (traversed.find(calledProcName) != traversed.end()) {
      continue;
    }
    traversed.emplace(calledProcName);

    // get callsStarProcs for this procedure, if applicable
    // a shortcut, basically
    try {
      for (const auto& called : callsStar.at(calledProcName)) {
        if (proc == called) {
          throw exceptions::SemanticError("Procedure call is recursive: " +
                                          proc);
        }
        if (traversed.find(called) == traversed.end()) {
          traverseQueue.push(called);
        }

        callsStarProcs->emplace(called);

        // assume that if it's in callsStar, it's been fully traversed
        traversed.emplace(called);
      }
    } catch (const std::out_of_range&) {
      // do nothing: probably means we haven't gotten to this proc yet
    }

    // then, check calls
    try {
      for (const auto& called : calls.at(calledProcName)) {
        if (proc == called) {
          throw exceptions::SemanticError("Procedure call is recursive: " +
                                          proc);
        }
        if (traversed.find(called) == traversed.end()) {
          traverseQueue.push(called);
        }

        callsStarProcs->emplace(called);
      }
    } catch (const std::out_of_range&) {
      // do nothing?
      // possibly means that the called procedure doesn't
      // call any other procedures
    }
  }
}

void ResolveCallsStar(
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        calls,
    std::unordered_map<std::string, std::unordered_set<std::string>>&
        callsStar) {
  // populate calls*
  for (const auto& [first, second] : calls) {
    ResolveCallsStarSingleProc(first, calls, callsStar);
  }
}

void ResolveCalledByStar(
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        callsStar,
    std::unordered_map<std::string, std::unordered_set<std::string>>&
        calledByStar) {
  // populate CalledByStar
  for (const auto& [first, second] : callsStar) {
    for (const auto& called : second) {
      calledByStar[called].emplace(first);
    }
  }
}

void ResolveFollowsRels(
    const std::vector<std::shared_ptr<rel::FollowsStmtStmtRelationship>>&
        followsRelationships,
    std::vector<int>& followsRels) {
  for (const auto& rel : followsRelationships) {
    followsRels[rel->secondStatementNumber()] = rel->firstStatementNumber();
  }
}

void ResolveParentRels(
    const std::vector<std::shared_ptr<rel::ParentStmtStmtRelationship>>&
        parentRelationships,
    std::vector<int>& parentRels) {
  for (const auto& rel : parentRelationships) {
    parentRels[rel->secondStatementNumber()] = rel->firstStatementNumber();
  }
}

void PopulateFollowsRels(const std::vector<int>& followsRels,
                         PopulateFacade* popFacade) {
  for (int i = 0; i < followsRels.size(); i++) {
    // for each stmt i...
    if (followsRels[i] != 0) {
      // store preceding(i) -> i follows and follows* relationship
      popFacade->storeFollowsRelationship(followsRels[i], i);
      popFacade->storeFollowsStarRelationship(followsRels[i], i);

      // store preceding(preceding(i)) -> i follows* relationship
      int nextRelIdx = followsRels[i];
      while (followsRels[nextRelIdx] != 0) {
        popFacade->storeFollowsStarRelationship(followsRels[nextRelIdx], i);
        nextRelIdx = followsRels[nextRelIdx];
      }
    }
  }
}

void PopulateParentRels(const std::vector<int>& parentRels,
                        PopulateFacade* popFacade) {
  for (int i = 0; i < parentRels.size(); i++) {
    // for each stmt i...
    if (parentRels[i] != 0) {
      // store parent(i) -> i parent and parent* relationship
      popFacade->storeParentRelationship(parentRels[i], i);
      popFacade->storeParentStarRelationship(parentRels[i], i);

      // store parent(parent(i)) -> i parent* relationship
      int nextRelIdx = parentRels[i];
      while (parentRels[nextRelIdx] != 0) {
        popFacade->storeParentStarRelationship(parentRels[nextRelIdx], i);
        nextRelIdx = parentRels[nextRelIdx];
      }
    }
  }
}

void PopulateCallRels(
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        calls,
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        callsStar,
    PopulateFacade* popFacade) {
  for (const auto& [first, second] : calls) {
    std::string callerProcName = first;
    for (const auto& calledProcName : second) {
      popFacade->storeCallsRelationship(callerProcName, calledProcName);
    }
  }
  for (const auto& [first, second] : callsStar) {
    std::string callerProcName = first;
    for (const auto& calledProcName : second) {
      popFacade->storeCallsStarRelationship(callerProcName, calledProcName);
    }
  }
}

void PopulateModifiesRels(
    const std::vector<std::shared_ptr<rel::ModifiesStmtVarRelationship>>&
        modifiesRelationships,
    const std::vector<int>& parentRels,
    const util::IntervalTree<int, std::string>& procNameByLine,
    const std::vector<std::shared_ptr<rel::CallStmtRelationship>>& callStmtRels,
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        calledByStar,
    PopulateFacade* popFacade) {
  std::unordered_map<std::string, std::unordered_set<std::string>>
      varModifiedByProc;
  for (const auto& rel : modifiesRelationships) {
    if (rel->relationshipType() != rel::RelationshipType::MODIFIES_STMT_VAR) {
      throw exceptions::ParserError("Invalid Modifies relationship type");
    }
    std::shared_ptr<rel::ModifiesStmtVarRelationship> modifiesRel =
        std::static_pointer_cast<rel::ModifiesStmtVarRelationship>(rel);

    std::string varName = modifiesRel->variableName();

    // store Modifies(stmt, v) and Modifies(container, v)
    int stmtNum = modifiesRel->statementNumber();
    while (stmtNum != 0) {
      popFacade->storeStatementModifiesVariableRelationship(stmtNum, varName);
      stmtNum = parentRels[stmtNum];
    }

    // store Modifies(proc, v) relations
    std::string procName =
        procNameByLine.Search(modifiesRel->statementNumber()).value();
    varModifiedByProc[procName].emplace(varName);
    popFacade->storeProcedureModifiesVariableRelationship(procName, varName);
    try {
      for (const auto& caller : calledByStar.at(procName)) {
        varModifiedByProc[caller].emplace(varName);
        popFacade->storeProcedureModifiesVariableRelationship(caller, varName);
      }
    } catch (const std::out_of_range&) {
      // do nothing
      // procName not in calledByStar -> procName is not called by
      // any other procedure, i.e. no Modifies(callerProc, v) relations
    }
  }

  // store Modifies(call, v) relations,
  // and more Modifies(container, v) relations
  for (const auto& callStmtRel : callStmtRels) {
    int stmtNum = callStmtRel->statementNumber();
    std::string procName = callStmtRel->procedureName();
    while (stmtNum != 0) {
      for (const auto& varName : varModifiedByProc[procName]) {
        popFacade->storeStatementModifiesVariableRelationship(stmtNum, varName);
      }
      stmtNum = parentRels[stmtNum];
    }
  }
}

void PopulateUsesRels(
    const std::vector<std::shared_ptr<rel::UsesStmtVarRelationship>>&
        usesRelationships,
    const std::vector<int>& parentRels,
    const util::IntervalTree<int, std::string>& procNameByLine,
    const std::vector<std::shared_ptr<rel::CallStmtRelationship>>& callStmtRels,
    const std::unordered_map<std::string, std::unordered_set<std::string>>&
        calledByStar,
    PopulateFacade* popFacade) {
  std::unordered_map<std::string, std::unordered_set<std::string>>
      varUsedByProc;
  for (const auto& rel : usesRelationships) {
    if (rel->relationshipType() != rel::RelationshipType::USES_STMT_VAR) {
      throw exceptions::ParserError("Invalid Uses relationship type");
    }

    std::shared_ptr<rel::UsesStmtVarRelationship> usesRel =
        std::static_pointer_cast<rel::UsesStmtVarRelationship>(rel);

    std::string varName = usesRel->variableName();

    // store Uses(stmt, v) + Uses(container, v)
    int stmtNum = usesRel->statementNumber();
    while (stmtNum != 0) {
      popFacade->storeStatementUsesVariableRelationship(stmtNum, varName);
      stmtNum = parentRels[stmtNum];
    }

    // store Uses(proc, v)
    std::string procName =
        procNameByLine.Search(usesRel->statementNumber()).value();
    varUsedByProc[procName].emplace(varName);
    popFacade->storeProcedureUsesVariableRelationship(procName, varName);
    try {
      for (const auto& caller : calledByStar.at(procName)) {
        varUsedByProc[caller].emplace(varName);
        popFacade->storeProcedureUsesVariableRelationship(caller, varName);
      }
    } catch (const std::out_of_range&) {
      // do nothing
      // procName not in calledByStar -> procName is not called by
      // any other procedure, i.e. no Uses(callerProc, v) relationships
    }
  }

  // store Uses(call, v), and
  // more Uses(container, v) relationships
  for (const auto& callStmtRel : callStmtRels) {
    int stmtNum = callStmtRel->statementNumber();
    std::string procName = callStmtRel->procedureName();
    while (stmtNum != 0) {
      for (const auto& varName : varUsedByProc[procName]) {
        popFacade->storeStatementUsesVariableRelationship(stmtNum, varName);
      }
      stmtNum = parentRels[stmtNum];
    }
  }
}

void PopulateAssignPostfixExpr(
    const std::vector<std::shared_ptr<rel::AssignExpRelationship>>&
        assignPostfixExprRels,
    PopulateFacade* popFacade) {
  for (const auto& rel : assignPostfixExprRels) {
    popFacade->storeAssignStatementPostfixExpression(rel->statementNumber(),
                                                     rel->postfixExp());
  }
}

bool SP::process(const std::string& program, PKB* pkb) const {
  // tokenize the string
  auto tokenizer = tokenizer::SimpleTokenizer();
  auto tokens = tokenizer.tokenize(program);

  // parse tokens into AST
  auto parser = parser::SimpleChainParser();
  auto ast = parser.Parse(std::move(tokens));

  auto programNode = ToProgramNode(ast->GetRoot());

  int totalStatementCount = programNode->GetTotalStatementCount();

  // process AST to get elements
  auto astElemRelationships =
      Visit<design_extractor::AstElemExtractor, rel::Relationship>(programNode);

  // process AST to find relationships
  auto followsRelationships =
      Visit<design_extractor::FollowsExtractor,
            rel::FollowsStmtStmtRelationship>(programNode);

  auto parentRelationships =
      Visit<design_extractor::ParentExtractor, rel::ParentStmtStmtRelationship>(
          programNode);

  auto modifiesRelationships =
      Visit<design_extractor::StmtModifiesExtractor,
            rel::ModifiesStmtVarRelationship>(programNode);

  auto usesRelationships =
      Visit<design_extractor::StmtUsesExtractor, rel::UsesStmtVarRelationship>(
          programNode);

  // process AST to get assign node <-> expression reprs
  auto assignExpRelationships =
      Visit<design_extractor::AssignExpExtractor, rel::AssignExpRelationship>(
          programNode);

  // postprocess relationships

  // temporary storage for call stmt rels for later processing
  // e.g. for Modifies/Uses(call, v)
  std::vector<std::shared_ptr<rel::CallStmtRelationship>> callStmtRels;
  ResolveCallStmtRels(astElemRelationships, callStmtRels);

  // procedureByName[procName] -> ProcedureNode mapping
  // procedureRels[stmtNum] = procName that stmtNum belongs to
  std::unordered_map<std::string, std::shared_ptr<ast::ProcedureNode>>
      procedureByName;
  util::IntervalTree<int, std::string> procNameByLine;
  ResolveProcedureInfo(programNode, procedureByName, procNameByLine);

  // calls[caller] = std::uset<std::string> of procs called by caller
  std::unordered_map<std::string, std::unordered_set<std::string>> calls;
  ResolveCalls(callStmtRels, calls);
  std::unordered_map<std::string, std::unordered_set<std::string>> callsStar;
  ResolveCallsStar(calls, callsStar);

  // for Uses, Modifies
  std::unordered_map<std::string, std::unordered_set<std::string>> calledByStar;
  ResolveCalledByStar(callsStar, calledByStar);

  // followsRels[secondStmtNum] = firstStmtNum (or 0 if it doesn't have
  // a preceding stmt)
  std::vector<int> followsRels;
  followsRels.resize(totalStatementCount + 1);
  ResolveFollowsRels(followsRelationships, followsRels);

  // parentRels[childStmtNum] = parentStmtNum (or 0 if it doesn't have a
  // parent stmt)
  std::vector<int> parentRels;
  parentRels.resize(totalStatementCount + 1);
  ResolveParentRels(parentRelationships, parentRels);

  // populate
  PopulateFacade* popFacade = pkb->getPopulateFacade();

  // put AST entities into PKB
  // and populate procedureByName, procNameByLine, callStmtRels
  PopulateAstEntities(astElemRelationships, popFacade);

  // store Follows, Follows* into PKB
  PopulateFollowsRels(followsRels, popFacade);

  // store Parent, Parent* into PKB
  PopulateParentRels(parentRels, popFacade);

  // store Calls, Calls* into PKB
  PopulateCallRels(calls, callsStar, popFacade);

  // store Modifies into PKB
  PopulateModifiesRels(modifiesRelationships, parentRels, procNameByLine,
                       callStmtRels, calledByStar, popFacade);

  // store Uses into PKB
  PopulateUsesRels(usesRelationships, parentRels, procNameByLine, callStmtRels,
                   calledByStar, popFacade);

  // store assign postfix exps into PKB
  PopulateAssignPostfixExpr(assignExpRelationships, popFacade);

  return true;
}
}  // namespace sp
