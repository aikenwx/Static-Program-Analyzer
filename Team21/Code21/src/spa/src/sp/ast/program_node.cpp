#include "program_node.h"

// Handles procedures like a stack LIFO
void ProgramNode::addProcedure(INode* procedure) {
  procedures.push_back(procedure);
}
