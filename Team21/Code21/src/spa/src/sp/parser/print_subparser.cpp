#include "sp/ast/astlib.h"
#include "print_subparser.h"
#include "util/instance_of.h"

namespace parser {
auto PrintSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  if (context->IsLookaheadSymbolValue(";")) {
    // print: 'print' iden ';'
    if (stack->size() >= 2
      && util::instance_of<ast::IdentifierNode>(*iter)
      && util::instance_of<ast::IdentifierNode>(*std::next(iter, 1)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(iter, 1)))->GetValue() == "print") {
      // References identifier node
      std::shared_ptr<ast::IdentifierNode> var =
          std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      // Pops identifier node
      stack->pop_back();
      // Pops 'print' identifier node
      stack->pop_back();
      // Creates print node
      std::shared_ptr<ast::PrintNode> sta =
          std::make_shared<ast::PrintNode>(var);
      context->GetStatementCounter()++;
      // Pushes print node to parse stack
      stack->push_back(sta);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}  // namespace parser
