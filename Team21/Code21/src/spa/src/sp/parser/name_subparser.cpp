#include "sp/ast/astlib.h"
#include "name_subparser.h"
#include "sp/token/identifier_token.h"
#include "util/instance_of.h"

namespace parser {
auto NameSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  if (!context->IsLookaheadTypeOf<token::IdentifierToken>()) {
    // A name is a wrapper to an identifier
    if (util::instance_of<ast::IdentifierNode>(*iter)) {
      // References identifier node
      std::shared_ptr<ast::IdentifierNode> ide =
          std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      // Pops identifier node
      stack->pop_back();
      // Creates name node
      std::shared_ptr<ast::NameNode> nam =
          std::make_shared<ast::NameNode>(ide->GetValue());
      // Pushes name node to parse stack
      stack->push_back(nam);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}  // namespace parser
