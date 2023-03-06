#include "sp.h"

#include <memory>
#include <queue>
#include <unordered_set>

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
#include "sp/parser/simple_parser.h"
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

bool SP::process(std::string program, PKB* pkb) {
  // tokenize the string
  auto tokenizer = tokenizer::SimpleTokenizer();
  std::vector<std::unique_ptr<token::Token>> tokens =
      tokenizer.tokenize(program);

  // parse tokens into AST
  auto parser = parser::SimpleParser();
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

  // put AST entities into PKB
  // and populate procedureByName, procedureRels, callStmtRels
  PopulateFacade* PopFacade = pkb->getPopulateFacade();
  for (const auto& rel : astElemRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::CALL_STMT) {
      std::shared_ptr<rel::CallStmtRelationship> callStmtRel =
          std::static_pointer_cast<rel::CallStmtRelationship>(rel);

      PopFacade->storeAssignmentStatement(callStmtRel->statementNumber());
      callStmtRels.push_back(callStmtRel);
    } else if (rel->relationshipType() == rel::RelationshipType::PROC) {
      std::shared_ptr<rel::ProcRelationship> procRel =
          std::static_pointer_cast<rel::ProcRelationship>(rel);
      std::string procName = procRel->procedureName();
      std::shared_ptr<ast::ProcedureNode> procNode = procRel->procedureNode();

      PopFacade->storeProcedure(procName);
      procedureByName[procName] = procNode;

      int startStmtNum = procNode->GetStartStatementNumber();
      int endStmtNum = procNode->GetEndStatementNumber();
      for (int i = startStmtNum; i <= endStmtNum; i++) {
        procedureRels[i] = procName;
      }
    } else if (rel->relationshipType() == rel::RelationshipType::ASSIGN_STMT) {
      PopFacade->storeAssignmentStatement(
          std::static_pointer_cast<rel::AssignStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::IF_STMT) {
      PopFacade->storeIfStatement(
          std::static_pointer_cast<rel::IfStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::WHILE_STMT) {
      PopFacade->storeWhileStatement(
          std::static_pointer_cast<rel::WhileStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::PRINT_STMT) {
      PopFacade->storePrintStatement(
          std::static_pointer_cast<rel::PrintStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::READ_STMT) {
      PopFacade->storeReadStatement(
          std::static_pointer_cast<rel::ReadStmtRelationship>(rel)
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::CONST) {
      PopFacade->storeConstant(
          std::static_pointer_cast<rel::ConstRelationship>(rel)->value());
    } else if (rel->relationshipType() == rel::RelationshipType::VAR) {
      PopFacade->storeVariable(
          std::static_pointer_cast<rel::VarRelationship>(rel)->variableName());
    }
  }

  // populate calls
  for (const auto& rel : callStmtRels) {
    std::string calledProcName = rel->procedureName();
    std::string callerProcName = procedureRels[rel->statementNumber()];

    calls[callerProcName].emplace(calledProcName);
  }

  // populate calls*
  for (const auto& [first, second] : calls) {
    std::unordered_set<std::string>* callsStarProcs = &callsStar[first];
    std::unordered_set<std::string> traversed;
    std::queue<std::string> traverseQueue;

    traverseQueue.push(first);
    while (!traverseQueue.empty()) {
      std::string calledProcName = traverseQueue.front();
      traverseQueue.pop();

      // if we haven't already traversed this procedure
      if (traversed.find(calledProcName) == traversed.end()) {
        traversed.emplace(calledProcName);

        // get callsStarProcs for this procedure, if applicable
        // a shortcut, basically
        if (callsStar.find(calledProcName) != callsStar.end()) {
          for (const auto& called : calls[calledProcName]) {
            if (traversed.find(called) == traversed.end()) {
              callsStarProcs->emplace(called);
              traverseQueue.push(called);

              // assume that if it's in callsStar, it's been fully traversed
              traversed.emplace(called);
            }
          }
        }

        // then, check calls
        if (calls.find(calledProcName) != calls.end()) {
          for (const auto& called : calls[calledProcName]) {
            if (traversed.find(called) == traversed.end()) {
              callsStarProcs->emplace(called);
              traverseQueue.push(called);
            }
          }
        }
      }
    }
  }

  // populate CalledByStar
  for (const auto& [first, second] : callsStar) {
    for (const auto& called : second) {
      calledByStar[called].emplace(first);
    }
  }

  // populate followsRels
  for (const auto& rel : followsRelationships) {
    std::shared_ptr<rel::FollowsStmtStmtRelationship> followsRel =
        std::static_pointer_cast<rel::FollowsStmtStmtRelationship>(rel);
    followsRels[followsRel->secondStatementNumber()] =
        followsRel->firstStatementNumber();
  }

  // populate parentRels
  for (const auto& rel : parentRelationships) {
    std::shared_ptr<rel::ParentStmtStmtRelationship> parentRel =
        std::static_pointer_cast<rel::ParentStmtStmtRelationship>(rel);
    parentRels[parentRel->secondStatementNumber()] =
        parentRel->firstStatementNumber();
  }

  // store Follows, Follows* into PKB
  for (int i = 0; i < followsRels.size(); i++) {
    // for each stmt i...
    if (followsRels[i] != 0) {
      // store preceding(i) -> i follows and follows* relationship
      PopFacade->storeFollowsRelationship(followsRels[i], i);
      PopFacade->storeFollowsStarRelationship(followsRels[i], i);

      // store preceding(preceding(i)) -> i follows* relationship
      int nextRelIdx = followsRels[i];
      while (followsRels[nextRelIdx] != 0) {
        PopFacade->storeFollowsStarRelationship(followsRels[nextRelIdx], i);
        nextRelIdx = followsRels[nextRelIdx];
      }
    }
  }

  // store Parent, Parent* into PKB
  for (int i = 0; i < parentRels.size(); i++) {
    // for each stmt i...
    if (parentRels[i] != 0) {
      // store parent(i) -> i parent and parent* relationship
      PopFacade->storeParentRelationship(parentRels[i], i);
      PopFacade->storeParentStarRelationship(parentRels[i], i);

      // store parent(parent(i)) -> i parent* relationship
      int nextRelIdx = parentRels[i];
      while (parentRels[nextRelIdx] != 0) {
        PopFacade->storeParentStarRelationship(parentRels[nextRelIdx], i);
        nextRelIdx = parentRels[nextRelIdx];
      }
    }
  }

  // store Calls, Calls* into PKB
  for (const auto& [first, second] : calls) {
    std::string callerProcName = first;
    for (const auto& calledProcName : second) {
      // TODO: uncomment when pr #193 merged
      // PopFacade->storeCallsRelationship(callerProcName, calledProcName);
    }
  }
  for (const auto& [first, second] : callsStar) {
    std::string callerProcName = first;
    for (const auto& calledProcName : second) {
      // TODO: uncomment when pr #193 merged
      // PopFacade->storeCallsStarRelationship(callerProcName, calledProcName);
    }
  }

  // store Modifies into PKB
  std::unordered_map<std::string, std::unordered_set<std::string>>
      varModifiedByProc;
  for (const auto& rel : modifiesRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::MODIFIES_STMT_VAR) {
      std::shared_ptr<rel::ModifiesStmtVarRelationship> modifiesRel =
          std::static_pointer_cast<rel::ModifiesStmtVarRelationship>(rel);

      std::string varName = modifiesRel->variableName();

      // store Modifies(stmt, v) and Modifies(container, v)
      int stmtNum = modifiesRel->statementNumber();
      while (stmtNum != 0) {
        PopFacade->storeStatementModifiesVariableRelationship(stmtNum, varName);
        stmtNum = parentRels[stmtNum];
      }

      // store Modifies(proc, v) relations
      std::string procName = procedureRels[modifiesRel->statementNumber()];
      varModifiedByProc[procName].emplace(varName);
      PopFacade->storeProcedureModifiesVariableRelationship(procName, varName);
      for (const auto& caller : calledByStar[procName]) {
        varModifiedByProc[caller].emplace(varName);
        PopFacade->storeProcedureModifiesVariableRelationship(caller, varName);
      }
    } else {
      throw std::runtime_error("Invalid Modifies relationship type");
    }
  }

  // store Modifies(call, v) relations
  for (const auto& callStmtRel : callStmtRels) {
    int stmtNum = callStmtRel->statementNumber();
    std::string procName = callStmtRel->procedureName();
    for (const auto& varName : varModifiedByProc[procName]) {
      PopFacade->storeStatementModifiesVariableRelationship(stmtNum, varName);
    }
  }

  // store Uses into PKB
  std::unordered_map<std::string, std::unordered_set<std::string>>
      varUsedByProc;
  for (const auto& rel : usesRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::USES_STMT_VAR) {
      std::shared_ptr<rel::UsesStmtVarRelationship> usesRel =
          std::static_pointer_cast<rel::UsesStmtVarRelationship>(rel);

      std::string varName = usesRel->variableName();

      // store Uses(stmt, v) + Uses(container, v)
      int stmtNum = usesRel->statementNumber();
      while (stmtNum != 0) {
        PopFacade->storeStatementUsesVariableRelationship(stmtNum, varName);
        stmtNum = parentRels[stmtNum];
      }

      // store Uses(proc, v)
      std::string procName = procedureRels[usesRel->statementNumber()];
      varUsedByProc[procName].emplace(varName);
      PopFacade->storeProcedureUsesVariableRelationship(procName, varName);
      for (const auto& caller : calledByStar[procName]) {
        varUsedByProc[caller].emplace(varName);
        PopFacade->storeProcedureUsesVariableRelationship(caller, varName);
      }
    } else {
      throw std::runtime_error("Invalid Uses relationship type");
    }
  }
  // store Uses(call, v)
  for (const auto& callStmtRel : callStmtRels) {
    int stmtNum = callStmtRel->statementNumber();
    std::string procName = callStmtRel->procedureName();
    for (const auto& varName : varUsedByProc[procName]) {
      PopFacade->storeStatementUsesVariableRelationship(stmtNum, varName);
    }
  }

  // store assign postfix exps into PKB
  for (const auto& rel : assignExpRelationships) {
    std::shared_ptr<rel::AssignExpRelationship> assignExpRel =
        std::static_pointer_cast<rel::AssignExpRelationship>(rel);
    PopFacade->storeAssignStatementPostfixExpression(
        assignExpRel->statementNumber(), assignExpRel->postfixExp());
  }

  return true;
}
}  // namespace sp
