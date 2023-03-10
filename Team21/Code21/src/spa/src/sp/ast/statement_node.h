#pragma once
#include "i_node.h"

namespace ast {
class StatementNode : public INode {
 public:
  int GetStatementNumber();
  void SetStatementNumber(int statementNumber);
  virtual void IncrementStatementNumber(int value);
  virtual std::string ToString() const override = 0;

  void AcceptVisitor(std::shared_ptr<INode> currentNode,
                     std::shared_ptr<design_extractor::Extractor> extractor,
                     int depth) override;

 protected:
  int statementNumber;
};
}  // namespace ast
