#pragma once

#include <vector>
#include "i_node.h"

class ProgramNode : public INode {
public:
  void addProcedure(INode *procedure);

private:
  std::vector<INode *> procedures;
};
