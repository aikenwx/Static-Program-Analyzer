#include "statement_list_subparser.h"

#include "sp/ast/astlib.h"
#include "sp/ast/container_statement_node.h"
#include "sp/ast/statement_list_node.h"
#include "sp/token/identifier_token.h"
#include "sp/token/right_brace_token.h"
#include "util/instance_of.h"

namespace parser {
auto StatementListSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  // stmtLst: stmt+ (adds container statement to statement list)
  if (context->IsLookaheadTypeOf<token::RightBraceToken>()
    || context->IsLookaheadTypeOf<token::IdentifierToken>()) {
    if (stack->size() >= 2
      && util::instance_of<ast::ContainerStatementNode>(*iter)
      && util::instance_of<ast::StatementListNode>(*std::next(iter, 1))) {
      // References (container) statement node
      std::shared_ptr<ast::StatementNode> sta =
          std::static_pointer_cast<ast::StatementNode>(stack->back());
      // Pops (container) statement node
      stack->pop_back();
      // References statement list node
      std::shared_ptr<ast::StatementListNode> lis =
          std::static_pointer_cast<ast::StatementListNode>(stack->back());
      // Adds (container) statement to statement list node
      lis->AddStatement(sta);
      // Maintains statement list node at top of stack
      return true;
    }
    // stmtLst: stmt+ (creates a one container statement statement list)
    if (util::instance_of<ast::ContainerStatementNode>(*iter)) {
      // References (container) statement node
      std::shared_ptr<ast::StatementNode> sta =
          std::static_pointer_cast<ast::StatementNode>(stack->back());
      // Pops (container) statement node
      stack->pop_back();
      // Creates statement list node
      std::shared_ptr<ast::StatementListNode> lis =
          std::make_shared<ast::StatementListNode>();
      // Adds (container) statement to statement list node
      lis->AddStatement(sta);
      // Pushes statement list node to parse stack
      stack->push_back(lis);
      return true;
    }
    // stmtLst: stmt+ (adds statement to statement list)
    if (stack->size() >= 3
      && util::instance_of<ast::SymbolNode>(*iter) && (std::static_pointer_cast<ast::SymbolNode>(*iter))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(iter, 1))
      && util::instance_of<ast::StatementListNode>(*std::next(iter, 2))) {
      // Pops semicolon symbol node
      stack->pop_back();
      // References statement node
      std::shared_ptr<ast::StatementNode> sta =
          std::static_pointer_cast<ast::StatementNode>(stack->back());
      // Pops statement node
      stack->pop_back();
      // References statement list node
      std::shared_ptr<ast::StatementListNode> lis =
          std::static_pointer_cast<ast::StatementListNode>(stack->back());
      // Sets statement number of statement node
      sta->SetStatementNumber(lis->GetEndStatementNumber() + 1);
      // Adds statement to statement list node
      lis->AddStatement(sta);
      // Maintains statement list node at top of stack
      return true;
    }
    // stmtLst: stmt+ (creates a one statement statement list)
    if (stack->size() >= 2
      && util::instance_of<ast::SymbolNode>(*iter) && (std::static_pointer_cast<ast::SymbolNode>(*iter))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(iter, 1))) {
      // Pops semicolon symbol node
      stack->pop_back();
      // References statement node
      std::shared_ptr<ast::StatementNode> sta =
          std::static_pointer_cast<ast::StatementNode>(stack->back());
      sta->SetStatementNumber(context->GetStatementCounter());
      // Pops statement node
      stack->pop_back();
      // Creates statement list node
      std::shared_ptr<ast::StatementListNode> lis =
          std::make_shared<ast::StatementListNode>();
      // Adds statement to statement list node
      lis->AddStatement(sta);
      // Pushes statement list node to parse stack
      stack->push_back(lis);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}  // namespace parser
