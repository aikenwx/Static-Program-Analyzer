#include "statement_node.h"

namespace ast {
int StatementNode::GetStatementNumber() { return statementNumber; }

void StatementNode::SetStatementNumber(int statementNumber) {
  this->statementNumber = statementNumber;
}

void StatementNode::IncrementStatementNumber(int value) {
  statementNumber += value;
}

void StatementNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  // default handling of statement nodes
  extractor->HandleStatementNode(
      std::static_pointer_cast<StatementNode>(currentNode), depth);
}
}  // namespace ast
