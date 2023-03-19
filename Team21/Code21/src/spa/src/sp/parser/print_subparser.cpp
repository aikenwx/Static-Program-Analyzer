#include "sp/ast/astlib.h"
#include "print_subparser.h"
#include "sp/token/semicolon_token.h"
#include "util/instance_of.h"

namespace parser {
bool PrintSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::SemicolonToken>()) {
    if (stack->size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && util::instance_of<ast::IdentifierNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(i, 1)))->GetValue() == "print") {
      // S(p) <- print V
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      stack->pop_back();
      stack->pop_back();
      std::shared_ptr<ast::PrintNode> p = std::make_shared<ast::PrintNode>(v);
      context->GetStatementCounter()++;
      stack->push_back(p);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
