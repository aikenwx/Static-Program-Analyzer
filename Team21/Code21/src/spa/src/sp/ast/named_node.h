#pragma once
#include <string>
#include "i_node.h"

namespace ast {
class NamedNode : public INode {
public:
 explicit NamedNode(std::string name);

 auto GetName() -> std::string;
 [[nodiscard]] auto ToString() const -> std::string override = 0;

protected:
  std::string name;
};
}  // namespace ast
