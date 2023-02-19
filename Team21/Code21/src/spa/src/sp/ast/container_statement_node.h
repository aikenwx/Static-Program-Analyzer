#pragma once
#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class ContainerStatementNode : public StatementNode {
public:
  virtual std::string ToString() const override = 0;
};
}
