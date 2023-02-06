#pragma once
#include <string>
#include "i_node.h"

namespace ast {
class NamedNode : public INode {
public:
  NamedNode(std::string name);

  std::string GetName();

protected:
  std::string name;
};
}
