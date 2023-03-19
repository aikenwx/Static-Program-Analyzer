#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class UnaryOperationNode : public INode {
public:
  explicit UnaryOperationNode(std::shared_ptr<INode> operand);

  [[nodiscard]] auto GetOperand() const -> std::shared_ptr<INode>;
  [[nodiscard]] auto ToString() const -> std::string override;

  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;

private:
  std::shared_ptr<INode> operand;
};
} // namespace ast
