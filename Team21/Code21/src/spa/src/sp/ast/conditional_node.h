#pragma once

#include "i_node.h"
#include "sp/ast/container_statement_node.h"

namespace ast {
class ConditionalNode : public ContainerStatementNode {
public:
  [[nodiscard]] virtual auto GetCondition() const -> std::shared_ptr<INode> = 0;
};
} // namespace ast
