#include "procedure_node.h"

namespace ast {
ProcedureNode::ProcedureNode(std::string name, StatementListNode *statements) : NamedNode(name) {
  this->statements = statements;
}

StatementListNode *ProcedureNode::GetStatements() {
  return statements;
}

std::ostream &ProcedureNode::Write(std::ostream &out) const {
  out << "procedure:" << name << "\n{";
  statements->Write(out);
  out << "}" << "\n";
  return out;
}
}
