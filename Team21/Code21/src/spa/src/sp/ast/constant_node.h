#pragma once
#include "i_node.h"

namespace ast {
class ConstantNode : public INode {
public:
 explicit ConstantNode(int value);

 [[nodiscard]] auto GetValue() const -> int;
 [[nodiscard]] auto ToString() const -> std::string override;

 void AcceptVisitor(std::shared_ptr<INode> currentNode,
                    std::shared_ptr<design_extractor::Extractor> extractor,
                    int depth) override;
private:
  int value;
};
}  // namespace ast
