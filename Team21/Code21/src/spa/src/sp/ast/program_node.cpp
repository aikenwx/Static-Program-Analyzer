#include "program_node.h"

namespace ast {
// Handles procedures like a stack LIFO
void ProgramNode::AddProcedure(INode* procedure) {
  procedures.push_back(procedure);
}
}
