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
  std::unique_ptr<ast::AST> ast = parser.Parse(tokens);

  // process AST to get elements
  std::vector<rel::Relationship *> astElemRelationships =
      design_extractor::Traverse(
          ast->GetRoot(), design_extractor::AstElemExtractor::GetInstance());

  // process AST to find relationships
  std::vector<rel::Relationship *> followsRelationships =
      design_extractor::Traverse(
          ast->GetRoot(), design_extractor::FollowsExtractor::GetInstance());
  std::vector<rel::Relationship *> parentRelationships =
      design_extractor::Traverse(
          ast->GetRoot(), design_extractor::ParentExtractor::GetInstance());
  std::vector<rel::Relationship *> modifiesRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          design_extractor::DirectlyModifiesExtractor::GetInstance());
  std::vector<rel::Relationship*> usesRelationships =
      design_extractor::Traverse(
          ast->GetRoot(),
          design_extractor::DirectlyUsesExtractor::GetInstance());

  // put AST entities into PKB
  PopulateFacade *PopFacade = pkb_->getPopulateFacade();
  for (auto rel : astElemRelationships) {
    // pretty nasty, but it'll work for now
    if (util::instance_of<rel::PrintStmtRelationship>(rel)) {
      PopFacade->storePrintStatement(
          static_cast<rel::PrintStmtRelationship *>(rel)->statementNumber());
    } else if (util::instance_of<rel::ReadStmtRelationship>(rel)) {
      PopFacade->storeReadStatement(
          static_cast<rel::ReadStmtRelationship *>(rel)->statementNumber());
    } else if (util::instance_of<rel::ConstRelationship>(rel)) {
      PopFacade->storeConstant(
          static_cast<rel::ConstRelationship *>(rel)->value());
    } else if (util::instance_of<rel::ProcRelationship>(rel)) {
      PopFacade->storeProcedure(
          static_cast<rel::ProcRelationship *>(rel)->procedureName());
    } else if (util::instance_of<rel::VarRelationship>(rel)) {
      PopFacade->storeVariable(
          static_cast<rel::VarRelationship *>(rel)->variableName());
    }
  }

  // put relationships into PKB
  for (auto &rel : followsRelationships) {
    rel::FollowsStmtStmtRelationship *followsRel =
        static_cast<rel::FollowsStmtStmtRelationship *>(rel);
    PopFacade->storeFollowsRelationship(followsRel->firstStatementNumber(), followsRel->secondStatementNumber());
  }

  for (auto &rel : parentRelationships) {
    rel::ParentStmtStmtRelationship *parentRel =
        static_cast<rel::ParentStmtStmtRelationship *>(rel);
    PopFacade->storeParentRelationship(parentRel->firstStatementNumber(), parentRel->secondStatementNumber());
  }

  for (auto &rel : modifiesRelationships) {
    if (util::instance_of<rel::ModifiesProcVarRelationship>(rel)) {
      rel::ModifiesProcVarRelationship *modifiesRel =
          static_cast<rel::ModifiesProcVarRelationship *>(rel);
      PopFacade->storeProcedureModifiesVariableRelationship(
          modifiesRel->procedureName(), modifiesRel->variableName());
    } else if (util::instance_of<rel::ModifiesStmtVarRelationship>(rel)) {
      rel::ModifiesStmtVarRelationship *modifiesRel =
          static_cast<rel::ModifiesStmtVarRelationship *>(rel);
      PopFacade->storeStatementModifiesVariableRelationship(modifiesRel->statementNumber(),
                                                            modifiesRel->variableName());
    }
  }

  for (auto &rel : usesRelationships) {
    if (util::instance_of<rel::UsesProcVarRelationship>(rel)) {
      rel::UsesProcVarRelationship* usesRel =
          static_cast<rel::UsesProcVarRelationship*>(rel);
      PopFacade->storeProcedureUsesVariableRelationship(
          usesRel->procedureName(), usesRel->variableName());
    } else if (util::instance_of<rel::UsesStmtVarRelationship>(rel)) {
      rel::UsesStmtVarRelationship *usesRel =
          static_cast<rel::UsesStmtVarRelationship *>(rel);
      PopFacade->storeStatementUsesVariableRelationship(usesRel->statementNumber(),
                                                        usesRel->variableName());
    }
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
  qps::QPS::evaluate(query, results, *pkb_->getQueryFacade());
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
