#include "procedure_node.h"

namespace ast {
ProcedureNode::ProcedureNode(std::string name, StatementListNode *statements) : NamedNode(name) {
  this->statements = statements;
}

StatementListNode *ProcedureNode::GetStatements() {
  return statements;
}

std::string ProcedureNode::ToString() const {
  return "procedure:\n{\n" + statements->ToString() + "}\n";
}
}
