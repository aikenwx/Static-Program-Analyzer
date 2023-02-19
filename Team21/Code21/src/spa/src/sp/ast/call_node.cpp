#include "call_node.h"

namespace ast {
CallNode::CallNode(std::shared_ptr<ProcedureNode> proc) {
  this->proc = proc;
}

std::shared_ptr<ProcedureNode> CallNode::GetProcedure() {
  return proc;
}

std::string CallNode::ToString() const {
  return "call:\n{\n" + proc->ToString() + "}\n";
}
}
