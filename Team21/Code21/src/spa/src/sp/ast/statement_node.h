#pragma once
#include "i_node.h"

namespace ast {
class StatementNode : public INode {

  virtual std::ostream &Write(std::ostream &out) const override = 0;
};
}
