#include "sp/ast/astlib.h"
#include "read_subparser.h"
#include "sp/token/semicolon_token.h"
#include "util/instance_of.h"

namespace parser {
auto ReadSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::SemicolonToken>()) {
    // read: 'read' iden ';'
    if (stack->size() >= 2
      && util::instance_of<ast::IdentifierNode>(*iter)
      && util::instance_of<ast::IdentifierNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(iter, 1)))->GetValue() == "read") {
      // References identifier node
      std::shared_ptr<ast::IdentifierNode> var =
          std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      // Pops identifier node
      stack->pop_back();
      // Pops 'read' identifier node
      stack->pop_back();
      // Creates read node
      std::shared_ptr<ast::ReadNode> sta = std::make_shared<ast::ReadNode>(var);
      context->GetStatementCounter()++;
      // Pushes read node to parse stack
      stack->push_back(sta);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}  // namespace parser
