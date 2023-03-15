#include "print_node.h"

#include <utility>

namespace ast {
PrintNode::PrintNode(std::shared_ptr<VariableNode> var)
    : var(std::move(std::move(var))) {}

auto PrintNode::GetVariable() -> std::shared_ptr<VariableNode> { return var; }

auto PrintNode::ToString() const -> std::string {
  return "print:\n{\n" + var->ToString() + "}\n";
}

void PrintNode::AcceptVisitor(
    const std::shared_ptr<INode>& currentNode,
    const std::shared_ptr<design_extractor::Extractor>& extractor, int depth) {
  extractor->HandleStatementNode(std::static_pointer_cast<StatementNode>(currentNode),
                                 depth);
  extractor->HandlePrintNode(std::static_pointer_cast<PrintNode>(currentNode),
                             depth);

  var->AcceptVisitor(var, extractor, depth + 1);
}
}  // namespace ast
