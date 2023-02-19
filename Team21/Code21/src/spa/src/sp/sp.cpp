#include "sp.h"

#include <memory>

#include "tokenizer/simple_tokenizer.h"
#include "sp/ast/ast.h"
#include "sp/design_extractor/ast_elem_extractor.h"
#include "sp/design_extractor/directly_modifies_extractor.h"
#include "sp/design_extractor/directly_uses_extractor.h"
#include "sp/design_extractor/follows_extractor.h"
#include "sp/design_extractor/parent_extractor.h"
#include "sp/design_extractor/traverse.h"
#include "sp/parser/simple_parser.h"
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
bool SP::process(std::string program, PKB* pkb) {
  // tokenize the string
  tokenizer::SimpleTokenizer tokenizer = tokenizer::SimpleTokenizer();
  std::vector<std::unique_ptr<token::Token>> tokens =
      tokenizer.tokenize(program);

  // parse tokens into AST
  parser::SimpleParser parser = parser::SimpleParser();
  std::unique_ptr<ast::AST> ast = parser.Parse(std::move(tokens));

  // process AST to get elements
  std::vector<std::unique_ptr<rel::Relationship>> astElemRelationships =
      design_extractor::Traverse(
          ast->GetRoot(), std::make_shared<design_extractor::AstElemExtractor>());

  // process AST to find relationships
  std::vector<std::unique_ptr<rel::Relationship>> followsRelationships =
      design_extractor::Traverse(
          ast->GetRoot(), std::make_shared<design_extractor::FollowsExtractor>());
  std::vector<std::unique_ptr<rel::Relationship>> parentRelationships =
      design_extractor::Traverse(
          ast->GetRoot(), std::make_shared<design_extractor::ParentExtractor>());
  std::vector<std::unique_ptr<rel::Relationship>> modifiesRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          std::make_shared<design_extractor::DirectlyModifiesExtractor>());
  std::vector<std::unique_ptr<rel::Relationship>> usesRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          std::make_shared<design_extractor::DirectlyUsesExtractor>());

  // put AST entities into PKB
  PopulateFacade* PopFacade = pkb->getPopulateFacade();
  for (auto&& rel : astElemRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::ASSIGN_STMT) {
      PopFacade->storeAssignmentStatement(
          std::static_pointer_cast<rel::AssignStmtRelationship>(std::move(rel))->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::CALL_STMT) {
      PopFacade->storeCallStatement(
          std::static_pointer_cast<rel::CallStmtRelationship>(std::move(rel))->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::IF_STMT) {
      PopFacade->storeIfStatement(
          std::static_pointer_cast<rel::IfStmtRelationship>(std::move(rel))->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::WHILE_STMT) {
      PopFacade->storeWhileStatement(
          std::static_pointer_cast<rel::WhileStmtRelationship>(std::move(rel))->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::PRINT_STMT) {
      PopFacade->storePrintStatement(
          std::static_pointer_cast<rel::PrintStmtRelationship>(std::move(rel))->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::READ_STMT) {
      PopFacade->storeReadStatement(
          std::static_pointer_cast<rel::ReadStmtRelationship>(std::move(rel))->statementNumber());
    } else if (rel->relationshipType() == rel::RelationshipType::CONST) {
      PopFacade->storeConstant(
          std::static_pointer_cast<rel::ConstRelationship>(std::move(rel))->value());
    } else if (rel->relationshipType() == rel::RelationshipType::PROC) {
      PopFacade->storeProcedure(
          std::static_pointer_cast<rel::ProcRelationship>(std::move(rel))->procedureName());
    } else if (rel->relationshipType() == rel::RelationshipType::VAR) {
      PopFacade->storeVariable(
          std::static_pointer_cast<rel::VarRelationship>(std::move(rel))->variableName());
    }
  }

  // put relationships into PKB
  // optimization: use vecs of ints because we're told that we won't have more than 500 stmts
  // also, add an extra 10% just in case

  // followsRels[secondStmtNum] = firstStmtNum (or 0 if it doesn't have a preceding stmt)
  std::vector<int> followsRels;
  followsRels.reserve(550);

  // parentRels[childStmtNum] = parentStmtNum (or 0 if it doesn't have a parent stmt)
  std::vector<int> parentRels;
  parentRels.reserve(550);

  for (auto& rel : followsRelationships) {
    std::unique_ptr<rel::FollowsStmtStmtRelationship> followsRel =
        std::static_pointer_cast<rel::FollowsStmtStmtRelationship>(std::move(rel));
    followsRels[followsRel->secondStatementNumber()] = followsRel->firstStatementNumber();
  }

  for (auto& rel : parentRelationships) {
    std::unique_ptr<rel::ParentStmtStmtRelationship> parentRel =
        std::static_pointer_cast<rel::ParentStmtStmtRelationship>(std::move(rel));
    parentRels[parentRel->secondStatementNumber()] = parentRel->firstStatementNumber();
  }

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

  for (auto& rel : modifiesRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::MODIFIES_PROC_VAR) {
      std::unique_ptr<rel::ModifiesProcVarRelationship> modifiesRel =
          std::static_pointer_cast<rel::ModifiesProcVarRelationship>(std::move(rel));
      PopFacade->storeProcedureModifiesVariableRelationship(
          modifiesRel->procedureName(), modifiesRel->variableName());
    } else if (rel->relationshipType() == rel::RelationshipType::MODIFIES_STMT_VAR) {
      std::unique_ptr<rel::ModifiesStmtVarRelationship> modifiesRel =
          std::static_pointer_cast<rel::ModifiesStmtVarRelationship>(std::move(rel));
      PopFacade->storeStatementModifiesVariableRelationship(
          modifiesRel->statementNumber(), modifiesRel->variableName());
    }
  }

  for (auto& rel : usesRelationships) {
    if (rel->relationshipType() == rel::RelationshipType::USES_PROC_VAR) {
      std::unique_ptr<rel::UsesProcVarRelationship> usesRel =
          std::static_pointer_cast<rel::UsesProcVarRelationship>(std::move(rel));
      PopFacade->storeProcedureUsesVariableRelationship(
          usesRel->procedureName(), usesRel->variableName());
    } else if (rel->relationshipType() == rel::RelationshipType::USES_STMT_VAR) {
      std::unique_ptr<rel::UsesStmtVarRelationship> usesRel =
          std::static_pointer_cast<rel::UsesStmtVarRelationship>(std::move(rel));
      PopFacade->storeStatementUsesVariableRelationship(
          usesRel->statementNumber(), usesRel->variableName());
    }
  }

  return true;
}
}  // namespace sp
