#pragma once
#include <vector>
#include "i_node.h"

namespace ast {
class ProgramNode : public INode {
public:
  void addProcedure(INode *procedure);

private:
  std::vector<INode *> procedures;
};
}
