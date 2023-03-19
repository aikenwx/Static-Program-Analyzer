#pragma once

#include <memory>
#include <utility>

#include "i_node.h"

namespace ast {
class BinaryOperationNode : public INode {
public:
  BinaryOperationNode(std::shared_ptr<INode> left,
                      std::shared_ptr<INode> right);

  [[nodiscard]] auto GetLeft() const -> std::shared_ptr<INode>;
  [[nodiscard]] auto GetRight() const -> std::shared_ptr<INode>;
  [[nodiscard]] auto ToString() const -> std::string override;
  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;

private:
  std::shared_ptr<INode> left_;
  std::shared_ptr<INode> right_;
};
} // namespace ast
