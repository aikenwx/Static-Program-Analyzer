#pragma once
#include "i_node.h"

namespace ast {
class StatementNode : public INode {
public:
  int GetStatementNumber();
  void SetStatementNumber(int statementNumber);
  virtual std::ostream &Write(std::ostream &out) const override = 0;

protected:
  int statementNumber;

};
}
