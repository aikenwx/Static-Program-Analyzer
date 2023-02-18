#pragma once

#include <memory>

#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class PrintNode : public StatementNode {
public:
  PrintNode(std::shared_ptr<VariableNode> var);

  std::shared_ptr<VariableNode> GetVariable();
  std::string ToString() const override;

private:
  std::shared_ptr<VariableNode> var;
};
}
