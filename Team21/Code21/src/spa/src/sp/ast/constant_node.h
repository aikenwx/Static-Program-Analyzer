#pragma once
#include "i_node.h"

namespace ast {
class ConstantNode : public INode {
public:
  ConstantNode(int value);

  int GetValue();
  std::string ToString() const override;

  void AcceptVisitor(std::shared_ptr<INode> currentNode,
                     std::shared_ptr<design_extractor::Extractor> extractor,
                     int depth) override;
private:
  int value;
};
}
