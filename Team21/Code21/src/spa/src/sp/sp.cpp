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

namespace sp {
bool VerifyAstRoot(std::shared_ptr<ast::INode> root) {
  if (!util::instance_of<ast::ProgramNode>(root)) {
    throw exceptions::SyntaxError("Invalid program");
  }

  std::shared_ptr<ast::ProgramNode> programNode =
      std::static_pointer_cast<ast::ProgramNode>(root);

  std::unordered_set<std::string> procedures;
  for (const auto& procNode : programNode->GetProcedures()) {
    if (procedures.find(procNode->GetName()) != procedures.end()) {
      throw exceptions::SemanticError("Duplicate procedure name");
    }
    procedures.insert(procNode->GetName());
  }
  return true;
}

void PopulateAstEntities(
    const std::vector<std::shared_ptr<rel::Relationship>>& astElemRelationships,
    std::unordered_map<std::string, std::shared_ptr<ast::ProcedureNode>>&
        procedureByName,
    std::vector<std::string>& procedureRels,
    std::vector<std::shared_ptr<rel::CallStmtRelationship>>& callStmtRels,
    PopulateFacade* popFacade) {
  for (const auto& rel : astElemRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::CALL_STMT) {
      std::shared_ptr<rel::CallStmtRelationship> callStmtRel =
          std::static_pointer_cast<rel::CallStmtRelationship>(rel);

      popFacade->storeAssignmentStatement(callStmtRel->statementNumber());
      callStmtRels.push_back(callStmtRel);
    } else if (rel->relationshipType() == rel::RelationshipType::PROC) {
      std::shared_ptr<rel::ProcRelationship> procRel =
          std::static_pointer_cast<rel::ProcRelationship>(rel);
      std::string procName = procRel->procedureName();
      std::shared_ptr<ast::ProcedureNode> procNode = procRel->procedureNode();

      popFacade->storeProcedure(procName);
      procedureByName[procName] = procNode;

      int startStmtNum = procNode->GetStartStatementNumber();
      int endStmtNum = procNode->GetEndStatementNumber();
      for (int i = startStmtNum; i <= endStmtNum; i++) {
        procedureRels[i] = procName;
      }
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

void ResolveCalls(
    const std::vector<std::shared_ptr<rel::CallStmtRelationship>>& callStmtRels,
    const std::vector<std::string>& procedureRels,
    std::unordered_map<std::string, std::unordered_set<std::string>>& calls) {
  // populate calls
  for (const auto& rel : callStmtRels) {
    std::string calledProcName = rel->procedureName();
    std::string callerProcName = procedureRels[rel->statementNumber()];

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
        if (traversed.find(called) == traversed.end()) {
          continue;
        }

        callsStarProcs->emplace(called);
        traverseQueue.push(called);

        // assume that if it's in callsStar, it's been fully traversed
        traversed.emplace(called);
      }
    } catch (const std::out_of_range&) {
      // do nothing: probably means we haven't gotten to this proc yet
    }

    // then, check calls
    try {
      for (const auto& called : calls.at(calledProcName)) {
        if (traversed.find(called) == traversed.end()) {
          callsStarProcs->emplace(called);
          traverseQueue.push(called);
        }
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
    const std::vector<std::string>& procedureRels,
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
    std::string procName = procedureRels[modifiesRel->statementNumber()];
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

  // store Modifies(call, v) relations
  for (const auto& callStmtRel : callStmtRels) {
    int stmtNum = callStmtRel->statementNumber();
    std::string procName = callStmtRel->procedureName();
    for (const auto& varName : varModifiedByProc[procName]) {
      popFacade->storeStatementModifiesVariableRelationship(stmtNum, varName);
    }
  }
}

void PopulateUsesRels(
    const std::vector<std::shared_ptr<rel::UsesStmtVarRelationship>>&
        usesRelationships,
    const std::vector<int>& parentRels,
    const std::vector<std::string>& procedureRels,
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
    std::string procName = procedureRels[usesRel->statementNumber()];
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

  // store Uses(call, v)
  for (const auto& callStmtRel : callStmtRels) {
    int stmtNum = callStmtRel->statementNumber();
    std::string procName = callStmtRel->procedureName();
    for (const auto& varName : varUsedByProc[procName]) {
      popFacade->storeStatementUsesVariableRelationship(stmtNum, varName);
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
  std::vector<std::unique_ptr<token::Token>> tokens =
      tokenizer.tokenize(program);

  // parse tokens into AST
  auto parser = parser::SimpleChainParser();
  std::shared_ptr<ast::AST> ast = parser.Parse(std::move(tokens));

  VerifyAstRoot(ast->GetRoot());

  std::shared_ptr<ast::ProgramNode> programNode =
      std::static_pointer_cast<ast::ProgramNode>(ast->GetRoot());

  int totalStatementCount = programNode->GetTotalStatementCount();

  // process AST to get elements
  auto astElemExtractor =
      std::make_shared<design_extractor::AstElemExtractor>();
  programNode->AcceptVisitor(programNode, astElemExtractor, 0);
  std::vector<std::shared_ptr<rel::Relationship>> astElemRelationships =
      astElemExtractor->GetRelationships();

  // process AST to find relationships
  auto followsExtractor =
      std::make_shared<design_extractor::FollowsExtractor>();
  programNode->AcceptVisitor(programNode, followsExtractor, 0);
  std::vector<std::shared_ptr<rel::FollowsStmtStmtRelationship>>
      followsRelationships = followsExtractor->GetRelationships();

  auto parentExtractor = std::make_shared<design_extractor::ParentExtractor>();
  programNode->AcceptVisitor(programNode, parentExtractor, 0);
  std::vector<std::shared_ptr<rel::ParentStmtStmtRelationship>>
      parentRelationships = parentExtractor->GetRelationships();

  auto stmtModifiesExtractor =
      std::make_shared<design_extractor::StmtModifiesExtractor>();
  programNode->AcceptVisitor(programNode, stmtModifiesExtractor, 0);
  std::vector<std::shared_ptr<rel::ModifiesStmtVarRelationship>>
      modifiesRelationships = stmtModifiesExtractor->GetRelationships();

  auto stmtUsesExtractor =
      std::make_shared<design_extractor::StmtUsesExtractor>();
  programNode->AcceptVisitor(programNode, stmtUsesExtractor, 0);
  std::vector<std::shared_ptr<rel::UsesStmtVarRelationship>> usesRelationships =
      stmtUsesExtractor->GetRelationships();

  // process AST to get assign node <-> expression reprs
  auto assignExpExtractor =
      std::make_shared<design_extractor::AssignExpExtractor>();
  programNode->AcceptVisitor(programNode, assignExpExtractor, 0);
  std::vector<std::shared_ptr<rel::AssignExpRelationship>>
      assignExpRelationships = assignExpExtractor->GetRelationships();

  // postprocess relationships
  // optimization?: use vecs of ints because we're told that we won't
  // have more than 500 stmts

  // followsRels[secondStmtNum] = firstStmtNum (or 0 if it doesn't have
  // a preceding stmt)
  std::vector<int> followsRels;
  followsRels.resize(totalStatementCount + 1);

  // parentRels[childStmtNum] = parentStmtNum (or 0 if it doesn't have a
  // parent stmt)
  std::vector<int> parentRels;
  parentRels.resize(totalStatementCount + 1);

  // procedureByName[procName] -> ProcedureNode mapping
  std::unordered_map<std::string, std::shared_ptr<ast::ProcedureNode>>
      procedureByName;

  // procedureRels[stmtNum] = procName that stmtNum belongs to
  //
  // using string here because i'm not sure if ast::ProcedureNode is
  // unique per ProcName
  //
  // TODO: consider using an interval/range tree
  std::vector<std::string> procedureRels;
  procedureRels.resize(totalStatementCount + 1);

  // calls[caller] = std::uset<std::string> of procs called by caller
  std::unordered_map<std::string, std::unordered_set<std::string>> calls;
  std::unordered_map<std::string, std::unordered_set<std::string>> callsStar;
  // for Uses, Modifies
  std::unordered_map<std::string, std::unordered_set<std::string>> calledByStar;

  // temporary storage for call stmt rels for later processing
  // e.g. for Modifies/Uses(call, v)
  std::vector<std::shared_ptr<rel::CallStmtRelationship>> callStmtRels;

  PopulateFacade* popFacade = pkb->getPopulateFacade();

  // put AST entities into PKB
  // and populate procedureByName, procedureRels, callStmtRels
  PopulateAstEntities(astElemRelationships, procedureByName, procedureRels,
                      callStmtRels, popFacade);

  // resolve calls, calls*, calledByStar
  ResolveCalls(callStmtRels, procedureRels, calls);
  ResolveCallsStar(calls, callsStar);
  ResolveCalledByStar(callsStar, calledByStar);

  // populate followsRels, parentRels
  ResolveFollowsRels(followsRelationships, followsRels);
  ResolveParentRels(parentRelationships, parentRels);

  // store Follows, Follows* into PKB
  PopulateFollowsRels(followsRels, popFacade);

  // store Parent, Parent* into PKB
  PopulateParentRels(parentRels, popFacade);

  // store Calls, Calls* into PKB
  PopulateCallRels(calls, callsStar, popFacade);

  // store Modifies into PKB
  PopulateModifiesRels(modifiesRelationships, parentRels, procedureRels,
                       callStmtRels, calledByStar, popFacade);

  // store Uses into PKB
  PopulateUsesRels(usesRelationships, parentRels, procedureRels, callStmtRels,
                   calledByStar, popFacade);

  // store assign postfix exps into PKB
  PopulateAssignPostfixExpr(assignExpRelationships, popFacade);

  return true;
}
}  // namespace sp
