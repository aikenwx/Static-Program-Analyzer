#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class BinaryOperationNode : public INode {
public:
  BinaryOperationNode(std::shared_ptr<INode> left, std::shared_ptr<INode> right);

  auto GetLeft() -> std::shared_ptr<INode>;
  auto GetRight() -> std::shared_ptr<INode>;
  [[nodiscard]] auto ToString() const -> std::string override = 0;
  void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                     const std::shared_ptr<design_extractor::Extractor>& extractor,
                     int depth) override;

 protected:
  std::shared_ptr<INode> left;
  std::shared_ptr<INode> right;
};
}  // namespace ast
