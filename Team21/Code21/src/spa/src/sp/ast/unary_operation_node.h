#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class UnaryOperationNode : public INode {
public:
  UnaryOperationNode(std::shared_ptr<INode> operand);

  std::shared_ptr<INode> GetOperand();
  virtual std::string ToString() const override = 0;

  void AcceptVisitor(std::shared_ptr<INode> currentNode,
                     std::shared_ptr<design_extractor::Extractor> extractor,
                     int depth) override;
protected:
  std::shared_ptr<INode> operand;
};
}
