#include "sp/ast/astlib.h"
#include "call_subparser.h"
#include "sp/token/semicolon_token.h"
#include "util/instance_of.h"

namespace parser {
bool CallSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::SemicolonToken>()) {
    if (stack->size() >= 2
      && util::instance_of<ast::NameNode>(*i)
      && util::instance_of<ast::IdentifierNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(i, 1)))->GetValue() == "call") {
      // S(c) <- call N
      std::shared_ptr<ast::NameNode> n = std::static_pointer_cast<ast::NameNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::CallNode> c = std::make_shared<ast::CallNode>(n);
      context->GetStatementCounter()++;
      stack->push_back(c);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
