#include "procedure_node.h"

ProcedureNode::ProcedureNode(std::string name, StatementListNode *statements) {
  this->name = name;
  this->statements = statements;
}

std::string ProcedureNode::getName() {
  return name;
}
StatementListNode *ProcedureNode::getStatements() {
  return statements;
}
