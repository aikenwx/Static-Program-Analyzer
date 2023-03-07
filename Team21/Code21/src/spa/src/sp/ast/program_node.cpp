#include "program_node.h"

namespace ast {
// Handles procedures like a stack LIFO
void ProgramNode::AddProcedure(std::shared_ptr<ProcedureNode> procedure) {
  procedures.push_back(procedure);
}

std::vector<std::shared_ptr<ProcedureNode>> ProgramNode::GetProcedures() {
  return procedures;
}

std::string ProgramNode::ToString() const {
  std::string str = "program:\n{\n";
  for (auto i = procedures.rbegin(); i < procedures.rend(); i++) {
    str += (*i)->ToString();
  }
  str += "}\n";
  return str;
}

int ProgramNode::GetTotalStatementCount() {
  return procedures.front()->GetEndStatementNumber();
}

bool ProgramNode::ContainsProcedure(std::string const &procedureName) {
  for (auto i = procedures.rbegin(); i < procedures.rend(); i++) {
    if ((*i)->GetName() == procedureName) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<ProcedureNode> ProgramNode::GetProcedure(std::string const &procedureName) {
  for (auto i = procedures.rbegin(); i < procedures.rend(); i++) {
    if ((*i)->GetName() == procedureName) {
      return *i;
    }
  }
}

void ProgramNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleProgramNode(
      std::static_pointer_cast<ProgramNode>(currentNode), depth);

  for (auto i = procedures.begin(); i < procedures.end(); i++) {
    (*i)->AcceptVisitor(*i, extractor, depth + 1);
  }
}
}  // namespace ast
