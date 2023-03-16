#include "sp/ast/astlib.h"
#include "statement_list_subparser.h"
#include "sp/token/right_brace_token.h"
#include "util/instance_of.h"

namespace parser {
bool StatementListSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::RightBraceToken>()) {
    // stmtLst: stmt+ (adds container statement to statement list)
    if (stack->size() >= 2
      && util::instance_of<ast::StatementListNode>(*i)
      && util::instance_of<ast::ContainerStatementNode>(*std::next(i, 1))) {
      // References statement list node
      std::shared_ptr<ast::StatementListNode> sl = std::static_pointer_cast<ast::StatementListNode>(stack->back());
      // Pops statement list node
      stack->pop_back();
      // References (container) statement node
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack->back());
      // Pops (container) statement node
      stack->pop_back();
      // Adds (container) statement to statement list node
      sl->AddStatement(s);
      // Pushes statement list node to parse stack
      stack->push_back(sl);
      return true;
    }
    // stmtLst: stmt+ (creates a one container statement statement list)
    if (util::instance_of<ast::ContainerStatementNode>(*i)) {
      // References (container) statement node
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack->back());
      // Pops (container) statement node
      stack->pop_back();
      // Creates statement list node
      std::shared_ptr<ast::StatementListNode> sl = std::make_shared<ast::StatementListNode>();
      // Adds (container) statement to statement list node
      sl->AddStatement(s);
      // Pushes statement list node to parse stack
      stack->push_back(sl);
      return true;
    }
    // stmtLst: stmt+ (adds statement to statement list)
    if (stack->size() >= 3
      && util::instance_of<ast::StatementListNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(i, 2))) {
      // References statement list node
      std::shared_ptr<ast::StatementListNode> sl = std::static_pointer_cast<ast::StatementListNode>(stack->back());
      // Pops statement list node
      stack->pop_back();
      // Pops semicolon symbol node
      stack->pop_back();
      // References statement node
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack->back());
      s->SetStatementNumber(sl->GetStartStatementNumber() - 1);
      // Pops statement node
      stack->pop_back();
      // Adds statement to statement list node
      sl->AddStatement(s);
      // Pushes statement list node to parse stack
      stack->push_back(sl);
      return true;
    }
    // stmtLst: stmt+ (creates a one statement statement list)
    if (stack->size() >= 2
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(i, 1))) {
      // Pops semicolon symbol node
      stack->pop_back();
      // References statement node
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack->back());
      s->SetStatementNumber(context->GetStatementCounter());
      // Pops statement node
      stack->pop_back();
      // Creates statement list node
      std::shared_ptr<ast::StatementListNode> sl = std::make_shared<ast::StatementListNode>();
      // Adds statement to statement list node
      sl->AddStatement(s);
      // Pushes statement list node to parse stack
      stack->push_back(sl);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
