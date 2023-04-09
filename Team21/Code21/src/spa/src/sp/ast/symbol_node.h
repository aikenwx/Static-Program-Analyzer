#pragma once

#include "i_node.h"

namespace ast {
class SymbolNode : public INode {
public:
 explicit SymbolNode(std::string_view type);

 auto GetType() -> std::string;
 [[nodiscard]] auto ToString() const -> std::string override;

private:
  std::string type_;
};
}  // namespace ast
