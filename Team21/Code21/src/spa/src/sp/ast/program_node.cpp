#include "program_node.h"

namespace ast {
// Handles procedures like a stack LIFO
void ProgramNode::AddProcedure(INode* procedure) {
  procedures.push_back(procedure);
}

std::vector<INode*> *ProgramNode::GetProcedures() {
  return &procedures;
}

std::ostream &ProgramNode::Write(std::ostream &out) const {
  out << "program:" << "\n{";
  for (auto i = procedures.rbegin(); i < procedures.rend(); i++) {
    (*i)->Write(out);
  }
  out << "}" << "\n";
  return out;
}
}
