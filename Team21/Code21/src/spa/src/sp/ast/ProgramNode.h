#pragma once

#include <vector>
#include "INode.h"

class ProgramNode : public INode {
private:
  std::vector<INode> procedures;
};