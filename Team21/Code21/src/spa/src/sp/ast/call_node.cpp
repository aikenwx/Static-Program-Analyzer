#include "call_node.h"

namespace ast {
CallNode::CallNode(std::shared_ptr<ProcedureNode> proc) { this->proc = proc; }

std::shared_ptr<ProcedureNode> CallNode::GetProcedure() { return proc; }

std::string CallNode::ToString() const {
  return "call:\n{\n" + proc->ToString() + "}\n";
}

void CallNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementNode(std::static_pointer_cast<StatementNode>(currentNode),
                                 depth);
  extractor->HandleCallNode(std::static_pointer_cast<CallNode>(currentNode),
                            depth);

  proc->AcceptVisitor(proc, extractor, depth + 1);
}
}  // namespace ast
