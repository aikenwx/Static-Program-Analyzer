#pragma once

#include <vector>
#include "INode.h"

class StatementListNode : public INode {
private:
  std::vector(INode) statements;
};