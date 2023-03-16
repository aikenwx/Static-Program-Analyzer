#include "sp/ast/astlib.h"
#include "print_subparser.h"
#include "sp/token/semicolon_token.h"
#include "util/instance_of.h"

namespace parser {
bool PrintSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::SemicolonToken>()) {
    // print: 'print' var_name';'
    if (stack->size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && util::instance_of<ast::IdentifierNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(i, 1)))->GetValue() == "print") {
      // References variable node
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      // Pops variable node
      stack->pop_back();
      // Pops 'print' identifier node
      stack->pop_back();
      // Creates print node
      std::shared_ptr<ast::PrintNode> p = std::make_shared<ast::PrintNode>(v);
      context->GetStatementCounter()++;
      // Pushes print node to parse stack
      stack->push_back(p);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
