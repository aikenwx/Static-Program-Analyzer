#include "sp/ast/astlib.h"
#include "name_subparser.h"
#include "sp/token/identifier_token.h"
#include "util/instance_of.h"

namespace parser {
bool NameSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (!context->IsLookaheadTypeOf<token::IdentifierToken>()) {
    if (util::instance_of<ast::IdentifierNode>(*i)) {
      // if not identifier then
      // N <- id
      std::shared_ptr<ast::IdentifierNode> id = std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::NameNode> n = std::make_shared<ast::NameNode>(id->GetValue());
      stack->push_back(n);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
