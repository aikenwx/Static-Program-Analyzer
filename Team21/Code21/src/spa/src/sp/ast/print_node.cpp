#include "print_node.h"

#include <utility>

namespace ast {
PrintNode::PrintNode(std::shared_ptr<IdentifierNode> var) : var(std::move(var)) {}

auto PrintNode::GetVariable() -> std::shared_ptr<IdentifierNode> { return var; }

auto PrintNode::ToString() const -> std::string {
  return "print:\n{\n" + var->ToString() + "}\n";
}

void PrintNode::AcceptVisitor(design_extractor::Extractor &extractor,
                              int depth) {
  auto currentNode = shared_from_this();
  extractor.HandleStatementNode(
      std::static_pointer_cast<StatementNode>(currentNode), depth);
  extractor.HandlePrintNode(std::static_pointer_cast<PrintNode>(currentNode),
                            depth);

  var->AcceptVisitor(extractor, depth + 1);
}
} // namespace ast
