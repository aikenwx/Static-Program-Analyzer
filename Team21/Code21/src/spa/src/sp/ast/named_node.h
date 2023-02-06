#pragma once
#include <string>
#include "i_node.h"

namespace ast {
class NamedNode : public INode {
public:
  NamedNode(std::string name);

  std::string GetName();
  virtual std::ostream &Write(std::ostream &out) const override = 0;

protected:
  std::string name;
};
}
