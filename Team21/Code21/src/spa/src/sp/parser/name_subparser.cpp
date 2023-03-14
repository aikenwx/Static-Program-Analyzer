#include "sp/ast/astlib.h"
#include "name_subparser.h"
#include "sp/token/identifier_token.h"
#include "util/instance_of.h"

namespace parser {
bool NameSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (!context->IsLookaheadTypeOf<token::IdentifierToken>()) {
    // N <- id
    if (util::instance_of<ast::IdentifierNode>(*i)) {
      // References identifier node
      std::shared_ptr<ast::IdentifierNode> id = std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      // Pops identifier node
      stack->pop_back();
      // Creates name node
      std::shared_ptr<ast::NameNode> n = std::make_shared<ast::NameNode>(id->GetValue());
      // Pushes name node to parse stack
      stack->push_back(n);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
