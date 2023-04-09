#pragma once
#include "statement_node.h"

namespace ast {
class ContainerStatementNode : public StatementNode {
public:
 [[nodiscard]] auto ToString() const -> std::string override = 0;
 [[nodiscard]] virtual auto GetEndStatementNumber() const -> int = 0;
};
}
