#pragma once
#include <string>
#include "i_node.h"

namespace ast {
class IdentifierNode : public INode {
public:
  explicit IdentifierNode(std::string value);

  auto GetValue() -> std::string;
  [[nodiscard]] auto ToString() const -> std::string override;

  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;
private:
  std::string value;
};
}  // namespace ast
