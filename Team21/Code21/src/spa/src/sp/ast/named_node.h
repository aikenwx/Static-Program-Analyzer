#pragma once
#include <string>
#include "i_node.h"

class NamedNode : public INode {
public:
  NamedNode(std::string name);

  std::string getName();

protected:
  std::string name;
};
