#include "procedure_node.h"

namespace ast {
ProcedureNode::ProcedureNode(std::string name, std::shared_ptr<StatementListNode> statements) : NamedNode(name) {
  this->statements = statements;
}

std::shared_ptr<StatementListNode> ProcedureNode::GetStatements() {
  return statements;
}

std::string ProcedureNode::ToString() const {
  return "procedure:\n{\n" + statements->ToString() + "}\n";
}

int ProcedureNode::GetStartStatementNumber() {
  return statements->GetStartStatementNumber();
}

int ProcedureNode::GetEndStatementNumber() {
  return statements->GetEndStatementNumber();
}
}
