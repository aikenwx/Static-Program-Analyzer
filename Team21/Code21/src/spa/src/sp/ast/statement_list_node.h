#pragma once
#include <memory>
#include <vector>

#include "statement_node.h"

namespace ast {
class StatementListNode : public INode {
public:
  void AddStatement(std::shared_ptr<StatementNode> node);
  // Format is a stack, i.e. statements are stored in reverse order
  std::vector<std::shared_ptr<StatementNode>> GetStatements();
  std::string ToString() const override;
  int GetStartStatementNumber();
  int GetEndStatementNumber();
  void IncrementStatementNumbers(int value);

private:
  std::vector<std::shared_ptr<StatementNode>> statements;
  int startStatementNumber;
  int endStatementNumber;
};
}
