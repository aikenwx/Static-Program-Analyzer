#pragma once

#include "i_node.h"
#include "sp/ast/container_statement_node.h"

namespace ast {
class ConditionalNode : public ContainerStatementNode {
public:
  virtual auto GetCondition() -> std::shared_ptr<INode> = 0;
};
} // namespace ast
