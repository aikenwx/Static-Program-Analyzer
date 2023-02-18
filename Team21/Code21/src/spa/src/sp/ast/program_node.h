#pragma once

#include <memory>
#include <vector>

#include "i_node.h"

namespace ast {
class ProgramNode : public INode {
public:
  void AddProcedure(std::shared_ptr<INode> procedure);
  std::vector<std::shared_ptr<INode>> GetProcedures();
  std::string ToString() const override;

private:
  std::vector<std::shared_ptr<INode> > procedures;
};
}
