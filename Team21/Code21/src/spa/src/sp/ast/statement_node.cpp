#include "statement_node.h"

namespace ast {
auto StatementNode::GetStatementNumber() const -> int {
  return statementNumber;
}

void StatementNode::SetStatementNumber(int statementNumber) {
  this->statementNumber = statementNumber;
}

void StatementNode::IncrementStatementNumber(int value) {
  statementNumber += value;
}

void StatementNode::AcceptVisitor(design_extractor::Extractor &extractor,
                                  int depth) {
  // default handling of statement nodes
  extractor.HandleStatementNode(
      std::static_pointer_cast<StatementNode>(shared_from_this()), depth);
}
} // namespace ast
