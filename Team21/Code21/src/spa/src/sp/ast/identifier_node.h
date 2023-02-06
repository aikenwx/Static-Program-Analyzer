#pragma once
#include <string>
#include "i_node.h"

namespace ast {
class IdentifierNode : public INode {
public:
  IdentifierNode(std::string value);

  void Execute();
  std::string GetValue();
  std::ostream &Write(std::ostream &out) const override;

private:
  std::string value;
};
}
