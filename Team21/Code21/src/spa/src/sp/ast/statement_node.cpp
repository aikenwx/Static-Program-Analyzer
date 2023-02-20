#include "statement_node.h"

namespace ast {
int StatementNode::GetStatementNumber() {
  return statementNumber;
}

void StatementNode::SetStatementNumber(int statementNumber) {
  this->statementNumber = statementNumber;
}

void StatementNode::IncrementStatementNumber(int value) {
  statementNumber += value;
}
}
