#include "call_node.h"

namespace ast {
CallNode::CallNode(std::shared_ptr<NameNode> name) {
  this->name = name->GetName();
}

std::string CallNode::GetProcedureName() {
  return name;
}

std::string CallNode::ToString() const {
  return "call:\n{\n" + name + "}\n";
}

void CallNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementNode(std::static_pointer_cast<StatementNode>(currentNode),
                                 depth);
  extractor->HandleCallNode(std::static_pointer_cast<CallNode>(currentNode),
                            depth);

  //proc->AcceptVisitor(proc, extractor, depth + 1);
}
}  // namespace ast
