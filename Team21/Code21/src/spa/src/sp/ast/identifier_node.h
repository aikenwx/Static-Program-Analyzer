#pragma once
#include <string>
#include "i_node.h"

class IdentifierNode : public INode {
public:
  IdentifierNode(std::string value);

  void execute();
  std::string getValue();

private:
  std::string value;
};
