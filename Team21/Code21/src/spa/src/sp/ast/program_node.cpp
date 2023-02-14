#include "program_node.h"

namespace ast {
// Handles procedures like a stack LIFO
void ProgramNode::AddProcedure(INode* procedure) {
  procedures.push_back(procedure);
}

std::vector<INode*> *ProgramNode::GetProcedures() {
  return &procedures;
}

std::string ProgramNode::ToString() const {
  std::string str = "program:\n{\n";
  for (auto i = procedures.rbegin(); i < procedures.rend(); i++) {
    str += (*i)->ToString();
  }
  str += "}\n";
  return str;
}
}
