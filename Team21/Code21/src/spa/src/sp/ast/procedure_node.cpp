#include "procedure_node.h"

namespace ast {
ProcedureNode::ProcedureNode(std::string name,
                             std::shared_ptr<StatementListNode> statements)
    : NamedNode(name) {
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

void ProcedureNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleProcedureNode(
      std::static_pointer_cast<ProcedureNode>(currentNode), depth);

  statements->AcceptVisitor(statements, extractor, depth + 1);
}
}  // namespace ast
