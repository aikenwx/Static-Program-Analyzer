#include "read_node.h"

namespace ast {
ReadNode::ReadNode(std::shared_ptr<VariableNode> var) { this->var = var; }

std::shared_ptr<VariableNode> ReadNode::GetVariable() { return var; }

std::string ReadNode::ToString() const {
  return "read:\n{\n" + var->ToString() + "}\n";
}

void ReadNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementNode(std::static_pointer_cast<StatementNode>(currentNode),
                                 depth);
  extractor->HandleReadNode(std::static_pointer_cast<ReadNode>(currentNode),
                            depth);

  var->AcceptVisitor(var, extractor, depth + 1);
}
}  // namespace ast
