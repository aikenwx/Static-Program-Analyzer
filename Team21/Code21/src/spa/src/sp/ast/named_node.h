#pragma once
#include <string>
#include "i_node.h"

namespace ast {
class NamedNode : public INode {
public:
 explicit NamedNode(std::string name);

 [[nodiscard]] auto GetName() const -> std::string;
 [[nodiscard]] auto ToString() const -> std::string override;

private:
  std::string name;
};
}  // namespace ast
