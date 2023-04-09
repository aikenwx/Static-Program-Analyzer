#include "sp/ast/astlib.h"
#include "read_subparser.h"
#include "util/instance_of.h"
#include "util/is_identifier_node_value.h"

namespace parser {
auto ReadSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  if (context->IsLookaheadSymbolValue(";")) {
    // read: 'read' iden ';'
    if (stack->size() >= 2
      && util::instance_of<ast::IdentifierNode>(*iter)
      && IsIdentifierNodeValue(*std::next(iter, 1), "read")) {
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
