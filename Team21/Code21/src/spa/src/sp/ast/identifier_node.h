#pragma once
#include <string>
#include "i_node.h"

namespace ast {
class IdentifierNode : public INode {
public:
 explicit IdentifierNode(std::string value);

 static void Execute();
 auto GetValue() -> std::string;
 [[nodiscard]] auto ToString() const -> std::string override;

private:
  std::string value;
};
}  // namespace ast
