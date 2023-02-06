#pragma once
#include <vector>
#include "i_node.h"

namespace ast {
class ProgramNode : public INode {
public:
  void AddProcedure(INode *procedure);

  std::ostream &Write(std::ostream &out) const override;

private:
  std::vector<INode *> procedures;
};
}
