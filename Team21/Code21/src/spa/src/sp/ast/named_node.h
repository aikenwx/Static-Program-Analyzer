#pragma once
#include <string>
#include "i_node.h"

namespace ast {
class NamedNode : public INode {
public:
  NamedNode(std::string name);

  std::string GetName();
  virtual std::string ToString() const override = 0;

protected:
  std::string name;
};
}
