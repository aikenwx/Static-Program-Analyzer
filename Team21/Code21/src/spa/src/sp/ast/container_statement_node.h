#pragma once
#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class ContainerStatementNode : public StatementNode {
public:
 [[nodiscard]] auto ToString() const -> std::string override = 0;
 virtual auto GetEndStatementNumber() -> int = 0;
 void IncrementStatementNumber(int value) override = 0;
};
}
