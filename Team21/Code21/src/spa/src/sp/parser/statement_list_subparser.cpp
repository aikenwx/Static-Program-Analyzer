#include "sp/ast/astlib.h"
#include "statement_list_subparser.h"
#include "token/right_brace_token.h"
#include "util/instance_of.h"

namespace parser {
bool StatementListSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::RightBraceToken>()) {
    // When lookahead is right brace (end of statement list)
    if (stack->size() >= 2
      && util::instance_of<ast::StatementListNode>(*i)
      && util::instance_of<ast::ContainerStatementNode>(*std::next(i, 1))) {
      // S+ <- Sc S+
      std::shared_ptr<ast::StatementListNode> sl = std::static_pointer_cast<ast::StatementListNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack->back());
      stack->pop_back();
      sl->AddStatement(s);
      stack->push_back(sl);
      return true;
    }
    if (util::instance_of<ast::ContainerStatementNode>(*i)) {
      // S+ <- Sc
      std::shared_ptr<ast::ContainerStatementNode> s = std::static_pointer_cast<ast::ContainerStatementNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::StatementListNode> sl = std::make_shared<ast::StatementListNode>();
      sl->AddStatement(s);
      stack->push_back(sl);
      return true;
    }
    if (stack->size() >= 3
      && util::instance_of<ast::StatementListNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(i, 2))) {
      // S+ <- S ; S+
      std::shared_ptr<ast::StatementListNode> sl = std::static_pointer_cast<ast::StatementListNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack->back());
      s->SetStatementNumber(sl->GetStartStatementNumber() - 1);
      stack->pop_back();
      sl->AddStatement(s);
      stack->push_back(sl);
      return true;
    } else if (stack->size() >= 2
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(i, 1))) {
      // S+ <- S ;
      stack->pop_back();
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack->back());
      s->SetStatementNumber(context->GetStatementCounter()++);
      stack->pop_back();
      std::shared_ptr<ast::StatementListNode> sl = std::make_shared<ast::StatementListNode>();
      sl->AddStatement(s);
      stack->push_back(sl);
      return true;
    }
  } else {
    return Subparser::Parse(context);
  }
}
}
