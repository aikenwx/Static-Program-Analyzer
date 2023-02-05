#include "procedure_node.h"

ProcedureNode::ProcedureNode(std::string name, StatementListNode *statements) : NamedNode(name) {
  this->statements = statements;
}

StatementListNode *ProcedureNode::getStatements() {
  return statements;
}
