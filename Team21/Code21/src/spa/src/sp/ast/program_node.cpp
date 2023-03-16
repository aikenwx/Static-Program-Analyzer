#include "program_node.h"

#include "exceptions/parser_error.h"

namespace ast {
// Handles procedures like a stack LIFO
void ProgramNode::AddProcedure(
    const std::shared_ptr<ProcedureNode>& procedure) {
  procedures.push_back(procedure);
}

auto ProgramNode::GetProcedures() const
    -> std::vector<std::shared_ptr<ProcedureNode>> {
  return procedures;
}

auto ProgramNode::ToString() const -> std::string {
  std::string str = "program:\n{\n";
  for (auto i = procedures.rbegin(); i < procedures.rend(); i++) {
    str += (*i)->ToString();
  }
  str += "}\n";
  return str;
}

auto ProgramNode::GetTotalStatementCount() -> int {
  return procedures.front()->GetEndStatementNumber();
}

auto ProgramNode::ContainsProcedure(const std::string& procedureName) -> bool {
  for (auto i = procedures.rbegin(); i < procedures.rend(); i++) {
    if ((*i)->GetName() == procedureName) {
      return true;
    }
  }
  return false;
}

auto ProgramNode::GetProcedure(const std::string& procedureName)
    -> std::shared_ptr<ProcedureNode> {
  for (auto i = procedures.rbegin(); i < procedures.rend(); i++) {
    if ((*i)->GetName() == procedureName) {
      return *i;
    }
  }

  throw exceptions::ParserError("Procedure for " + procedureName + " not found while trying to validate procedure");
}

void ProgramNode::AcceptVisitor(
    const std::shared_ptr<INode>& currentNode,
    const std::shared_ptr<design_extractor::Extractor>& extractor, int depth) {
  extractor->HandleProgramNode(
      std::static_pointer_cast<ProgramNode>(currentNode), depth);

  for (auto i = procedures.begin(); i < procedures.end(); i++) {
    (*i)->AcceptVisitor(*i, extractor, depth + 1);
  }
}
}  // namespace ast
