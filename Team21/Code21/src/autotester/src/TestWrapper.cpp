#include "TestWrapper.h"

#include <fstream>
#include <sstream>

#include "PKB/PKB.h"
#include "query/query_exceptions.h"
#include "query_evaluators/QPS.h"
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

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() : pkb_() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  pkb_ = new PKB();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // read file into string
  std::ifstream t(filename);
  std::stringstream buf;
  buf << t.rdbuf();
  std::string program = buf.str();

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
  PopulateFacade* PopFacade = pkb_->getPopulateFacade();
  for (auto&& rel : astElemRelationships) {
    // pretty nasty, but it'll work for now
    if (util::instance_of<rel::PrintStmtRelationship>(rel)) {
      PopFacade->storePrintStatement(
          std::static_pointer_cast<rel::PrintStmtRelationship>(std::move(rel))->statementNumber());
    } else if (util::instance_of<rel::ReadStmtRelationship>(rel)) {
      PopFacade->storeReadStatement(
          std::static_pointer_cast<rel::ReadStmtRelationship>(std::move(rel))->statementNumber());
    } else if (util::instance_of<rel::ConstRelationship>(rel)) {
      PopFacade->storeConstant(
          std::static_pointer_cast<rel::ConstRelationship>(std::move(rel))->value());
    } else if (util::instance_of<rel::ProcRelationship>(rel)) {
      PopFacade->storeProcedure(
          std::static_pointer_cast<rel::ProcRelationship>(std::move(rel))->procedureName());
    } else if (util::instance_of<rel::VarRelationship>(rel)) {
      PopFacade->storeVariable(
          std::static_pointer_cast<rel::VarRelationship>(std::move(rel))->variableName());
    }
  }

  // put relationships into PKB
  for (auto& rel : followsRelationships) {
    std::unique_ptr<rel::FollowsStmtStmtRelationship> followsRel =
        std::static_pointer_cast<rel::FollowsStmtStmtRelationship>(std::move(rel));
    PopFacade->storeFollowsRelationship(
        followsRel->firstStatementNumber(), followsRel->firstEntityType(),
        followsRel->secondStatementNumber(), followsRel->secondEntityType());
  }

  for (auto& rel : parentRelationships) {
    std::unique_ptr<rel::ParentStmtStmtRelationship> parentRel =
        std::static_pointer_cast<rel::ParentStmtStmtRelationship>(std::move(rel));
    PopFacade->storeParentRelationship(
        parentRel->firstStatementNumber(), parentRel->firstEntityType(),
        parentRel->secondStatementNumber(), parentRel->secondEntityType());
  }

  for (auto& rel : modifiesRelationships) {
    if (util::instance_of<rel::ModifiesProcVarRelationship>(rel)) {
      std::unique_ptr<rel::ModifiesProcVarRelationship> modifiesRel =
          std::static_pointer_cast<rel::ModifiesProcVarRelationship>(std::move(rel));
      PopFacade->storeProcedureModifiesVariableRelationship(
          modifiesRel->procedureName(), modifiesRel->variableName());
    } else if (util::instance_of<rel::ModifiesStmtVarRelationship>(rel)) {
      std::unique_ptr<rel::ModifiesStmtVarRelationship> modifiesRel =
          std::static_pointer_cast<rel::ModifiesStmtVarRelationship>(std::move(rel));
      PopFacade->storeStatementModifiesVariableRelationship(
          modifiesRel->statementNumber(), modifiesRel->entityType(),
          modifiesRel->variableName());
    }
  }

  for (auto& rel : usesRelationships) {
    if (util::instance_of<rel::UsesProcVarRelationship>(rel)) {
      std::unique_ptr<rel::UsesProcVarRelationship> usesRel =
          std::static_pointer_cast<rel::UsesProcVarRelationship>(std::move(rel));
      PopFacade->storeProcedureUsesVariableRelationship(
          usesRel->procedureName(), usesRel->variableName());
    } else if (util::instance_of<rel::UsesStmtVarRelationship>(rel)) {
      std::unique_ptr<rel::UsesStmtVarRelationship> usesRel =
          std::static_pointer_cast<rel::UsesStmtVarRelationship>(std::move(rel));
      PopFacade->storeStatementUsesVariableRelationship(
          usesRel->statementNumber(), usesRel->entityType(),
          usesRel->variableName());
    }
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  qps::QPS::evaluate(query, results, *pkb_->getQueryFacade());
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
