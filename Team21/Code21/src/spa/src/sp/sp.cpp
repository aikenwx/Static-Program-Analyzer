#include "sp.h"

#include <memory>
#include <queue>
#include <unordered_set>

#include "exceptions/semantic_error.h"
#include "exceptions/syntax_error.h"
#include "sp/ast/ast.h"
#include "sp/design_extractor/assign_exp_extractor.h"
#include "sp/design_extractor/ast_elem_extractor.h"
#include "sp/design_extractor/follows_extractor.h"
#include "sp/design_extractor/parent_extractor.h"
#include "sp/design_extractor/stmt_modifies_extractor.h"
#include "sp/design_extractor/stmt_uses_extractor.h"
#include "sp/design_extractor/traverse.h"
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
#include "util/unique_ptr_cast.h"

namespace sp {
bool VerifyAstRoot(std::shared_ptr<ast::INode> root) {
  if (!util::instance_of<ast::ProgramNode>(root)) {
    throw exceptions::SyntaxError("Unknown syntax error");
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
  tokenizer::SimpleTokenizer tokenizer = tokenizer::SimpleTokenizer();
  std::vector<std::unique_ptr<token::Token>> tokens =
      tokenizer.tokenize(program);

  // parse tokens into AST
  parser::SimpleParser parser = parser::SimpleParser();
  std::unique_ptr<ast::AST> ast = parser.Parse(std::move(tokens));

  VerifyAstRoot(ast->GetRoot());

  std::shared_ptr<ast::ProgramNode> programNode =
      std::static_pointer_cast<ast::ProgramNode>(ast->GetRoot());

  int totalStatementCount = programNode->GetTotalStatementCount();

  // process AST to get elements
  std::vector<std::unique_ptr<rel::Relationship>> astElemRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          std::make_shared<design_extractor::AstElemExtractor>());

  // process AST to find relationships
  std::vector<std::unique_ptr<rel::Relationship>> followsRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          std::make_shared<design_extractor::FollowsExtractor>());
  std::vector<std::unique_ptr<rel::Relationship>> parentRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          std::make_shared<design_extractor::ParentExtractor>());
  std::vector<std::unique_ptr<rel::Relationship>> modifiesRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          std::make_shared<design_extractor::StmtModifiesExtractor>());
  std::vector<std::unique_ptr<rel::Relationship>> usesRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          std::make_shared<design_extractor::StmtUsesExtractor>());

  // process AST to get assign node <-> expression reprs
  std::vector<std::unique_ptr<rel::Relationship>> assignExpRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          std::make_shared<design_extractor::AssignExpExtractor>());

  // postprocess relationships
  // optimization?: use vecs of ints because we're told that we won't have more
  // than 500 stmts

  // followsRels[secondStmtNum] = firstStmtNum (or 0 if it doesn't have a
  // preceding stmt)
  std::vector<int> followsRels;
  followsRels.resize(totalStatementCount + 1);

  // parentRels[childStmtNum] = parentStmtNum (or 0 if it doesn't have a parent
  // stmt)
  std::vector<int> parentRels;
  parentRels.resize(totalStatementCount + 1);

  // procedureByName[procName] = ProcedureNode
  std::unordered_map<std::string, std::shared_ptr<ast::ProcedureNode>>
      procedureByName;

  // procedureRels[stmtNum] = procName that stmtNum belongs to
  //
  // using string here because i'm not sure if ast::ProcedureNode is unique per
  // ProcName
  //
  // TODO: consider using an interval/range tree
  std::vector<std::string> procedureRels;
  procedureRels.resize(totalStatementCount + 1);

  // calledBy[calledProcNode] = std::vec<ProcedureNode> that call calledProcNode
  // it's kinda icky, but...
  std::unordered_map<std::string, std::unordered_set<std::string>> calledBy;

  // temporary storage for call stmt rels for later processing
  // only needed until calledBy is fully populated
  std::vector<std::unique_ptr<rel::CallStmtRelationship>> callStmtRels;

  // put AST entities into PKB
  // and populate procedureByName, procedureRels, callStmtRels
  PopulateFacade* PopFacade = pkb->getPopulateFacade();
  for (auto&& rel : astElemRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::CALL_STMT) {
      std::unique_ptr<rel::CallStmtRelationship> callStmtRel =
          std::static_pointer_cast<rel::CallStmtRelationship>(std::move(rel));

      PopFacade->storeAssignmentStatement(callStmtRel->statementNumber());
      callStmtRels.push_back(std::move(callStmtRel));
    } else if (rel->relationshipType() == rel::RelationshipType::PROC) {
      std::unique_ptr<rel::ProcRelationship> procRel =
          std::static_pointer_cast<rel::ProcRelationship>(std::move(rel));
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
          std::static_pointer_cast<rel::AssignStmtRelationship>(std::move(rel))
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::IF_STMT) {
      PopFacade->storeIfStatement(
          std::static_pointer_cast<rel::IfStmtRelationship>(std::move(rel))
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::WHILE_STMT) {
      PopFacade->storeWhileStatement(
          std::static_pointer_cast<rel::WhileStmtRelationship>(std::move(rel))
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::PRINT_STMT) {
      PopFacade->storePrintStatement(
          std::static_pointer_cast<rel::PrintStmtRelationship>(std::move(rel))
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::READ_STMT) {
      PopFacade->storeReadStatement(
          std::static_pointer_cast<rel::ReadStmtRelationship>(std::move(rel))
              ->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::CONST) {
      PopFacade->storeConstant(
          std::static_pointer_cast<rel::ConstRelationship>(std::move(rel))
              ->value());
    } else if (rel->relationshipType() == rel::RelationshipType::VAR) {
      PopFacade->storeVariable(
          std::static_pointer_cast<rel::VarRelationship>(std::move(rel))
              ->variableName());
    }
  }

  // populate calledBy
  for (const auto& rel : callStmtRels) {
    std::string calledProcName = rel->procedureName();
    std::string callerProcName = procedureRels[rel->statementNumber()];

    calledBy[calledProcName].emplace(callerProcName);
  }

  // populate followsRels
  for (auto& rel : followsRelationships) {
    std::unique_ptr<rel::FollowsStmtStmtRelationship> followsRel =
        std::static_pointer_cast<rel::FollowsStmtStmtRelationship>(
            std::move(rel));
    followsRels[followsRel->secondStatementNumber()] =
        followsRel->firstStatementNumber();
  }

  // populate parentRels
  for (auto& rel : parentRelationships) {
    std::unique_ptr<rel::ParentStmtStmtRelationship> parentRel =
        std::static_pointer_cast<rel::ParentStmtStmtRelationship>(
            std::move(rel));
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

  // store Modifies into PKB
  for (auto& rel : modifiesRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::MODIFIES_STMT_VAR) {
      std::unique_ptr<rel::ModifiesStmtVarRelationship> modifiesRel =
          std::static_pointer_cast<rel::ModifiesStmtVarRelationship>(
              std::move(rel));

      std::string varName = modifiesRel->variableName();

      // store Modifies(stmt, v) + Modifies(container, v)
      int stmtNum = modifiesRel->statementNumber();
      while (stmtNum != 0) {
        PopFacade->storeStatementModifiesVariableRelationship(stmtNum, varName);
        stmtNum = parentRels[stmtNum];
      }

      // store Modifies(proc, v)
      std::string procName = procedureRels[modifiesRel->statementNumber()];
      std::unordered_set<std::string> visitedProcs;
      std::queue<std::string> procQueue;
      procQueue.push(procName);
      while (!procQueue.empty()) {
        std::string currProc = procQueue.front();
        procQueue.pop();
        visitedProcs.emplace(currProc);
        PopFacade->storeProcedureModifiesVariableRelationship(currProc,
                                                              varName);

        for (const auto& caller : calledBy[currProc]) {
          if (visitedProcs.find(caller) == visitedProcs.end()) {
            procQueue.push(caller);
          }
        }
      }

      // store Modifies(call, v)...?
      // TODO
    } else {
      throw std::runtime_error("Invalid Modifies relationship type");
    }
  }

  // store Uses into PKB
  for (auto& rel : usesRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::USES_STMT_VAR) {
      std::unique_ptr<rel::UsesStmtVarRelationship> usesRel =
          std::static_pointer_cast<rel::UsesStmtVarRelationship>(
              std::move(rel));

      std::string varName = usesRel->variableName();

      // store Uses(stmt, v) + Uses(container, v)
      int stmtNum = usesRel->statementNumber();
      while (stmtNum != 0) {
        PopFacade->storeStatementUsesVariableRelationship(stmtNum, varName);
        stmtNum = parentRels[stmtNum];
      }

      // store Uses(proc, v)
      std::string procName = procedureRels[usesRel->statementNumber()];
      std::unordered_set<std::string> visitedProcs;
      std::queue<std::string> procQueue;
      procQueue.push(procName);
      while (!procQueue.empty()) {
        std::string currProc = procQueue.front();
        procQueue.pop();
        visitedProcs.emplace(currProc);
        PopFacade->storeProcedureUsesVariableRelationship(currProc, varName);

        for (const auto& caller : calledBy[currProc]) {
          if (visitedProcs.find(caller) == visitedProcs.end()) {
            procQueue.push(caller);
          }
        }
      }

      // store Uses(call, v)...?
      // TODO
    } else {
      throw std::runtime_error("Invalid Uses relationship type");
    }
  }

  // store assign postfix exps into PKB
  for (auto& rel : assignExpRelationships) {
    std::unique_ptr<rel::AssignExpRelationship> assignExpRel =
        std::static_pointer_cast<rel::AssignExpRelationship>(std::move(rel));
    PopFacade->storeAssignStatementPostfixExpression(
        assignExpRel->statementNumber(), assignExpRel->postfixExp());
  }

  return true;
}
}  // namespace sp
