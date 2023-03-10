#include "print_node.h"

namespace ast {
PrintNode::PrintNode(std::shared_ptr<VariableNode> var) { this->var = var; }

std::shared_ptr<VariableNode> PrintNode::GetVariable() { return var; }

std::string PrintNode::ToString() const {
  return "print:\n{\n" + var->ToString() + "}\n";
}

void PrintNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementNode(std::static_pointer_cast<StatementNode>(currentNode),
                                 depth);
  extractor->HandlePrintNode(std::static_pointer_cast<PrintNode>(currentNode),
                             depth);

  var->AcceptVisitor(var, extractor, depth + 1);
}
}  // namespace ast
