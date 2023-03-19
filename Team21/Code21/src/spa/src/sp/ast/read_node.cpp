#include "read_node.h"

#include <utility>

namespace ast {
ReadNode::ReadNode(std::shared_ptr<VariableNode> var) : var(std::move(var)) {}

auto ReadNode::GetVariable() -> std::shared_ptr<VariableNode> { return var; }

auto ReadNode::ToString() const -> std::string {
  return "read:\n{\n" + var->ToString() + "}\n";
}

void ReadNode::AcceptVisitor(design_extractor::Extractor &extractor,
                             int depth) {
  auto currentNode = shared_from_this();
  extractor.HandleStatementNode(
      std::static_pointer_cast<StatementNode>(currentNode), depth);
  extractor.HandleReadNode(std::static_pointer_cast<ReadNode>(currentNode),
                           depth);

  var->AcceptVisitor(extractor, depth + 1);
}
} // namespace ast
