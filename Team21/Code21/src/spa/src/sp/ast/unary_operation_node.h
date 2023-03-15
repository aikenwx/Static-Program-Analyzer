#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class UnaryOperationNode : public INode {
public:
 explicit UnaryOperationNode(std::shared_ptr<INode> operand);

 auto GetOperand() -> std::shared_ptr<INode>;
 [[nodiscard]] auto ToString() const -> std::string override = 0;

 void AcceptVisitor(std::shared_ptr<INode> currentNode,
                    std::shared_ptr<design_extractor::Extractor> extractor,
                    int depth) override;
protected:
  std::shared_ptr<INode> operand;
};
}  // namespace ast
