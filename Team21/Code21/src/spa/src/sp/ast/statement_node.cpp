#include "statement_node.h"

namespace ast {
int StatementNode::GetStatementNumber() {
  return statementNumber;
}

void StatementNode::SetStatementNumber(int statementNumber) {
  this->statementNumber = statementNumber;
}
}
