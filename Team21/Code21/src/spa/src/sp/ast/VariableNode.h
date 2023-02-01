#pragma once

#include <string>
#include "INode.h"

class VariableNode : public INode {
private:
  std::string name;
};