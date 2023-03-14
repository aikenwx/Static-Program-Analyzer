#include "sp/ast/astlib.h"
#include "read_subparser.h"
#include "sp/token/semicolon_token.h"
#include "util/instance_of.h"

namespace parser {
bool ReadSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::SemicolonToken>()) {
    // S(r) <- read V
    if (stack->size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && util::instance_of<ast::IdentifierNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(i, 1)))->GetValue() == "read") {
      // References variable node
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      // Pops variable node
      stack->pop_back();
      // Pops 'read' identifier node
      stack->pop_back();
      // Creates read node
      std::shared_ptr<ast::ReadNode> r = std::make_shared<ast::ReadNode>(v);
      context->GetStatementCounter()++;
      // Pushes read node to parse stack
      stack->push_back(r);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
