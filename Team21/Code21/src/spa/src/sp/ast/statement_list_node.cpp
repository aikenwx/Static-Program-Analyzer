#include "statement_list_node.h"

#include "container_statement_node.h"
#include "util/instance_of.h"

namespace ast {
void StatementListNode::AddStatement(
    const std::shared_ptr<StatementNode>& node) {
  if (statements.empty()) {
    if (util::instance_of<ContainerStatementNode>(node)) {
      endStatementNumber =
          std::static_pointer_cast<ContainerStatementNode>(node)
              ->GetEndStatementNumber();
    } else {
      endStatementNumber = node->GetStatementNumber();
    }
  }
  startStatementNumber = node->GetStatementNumber();
  statements.push_back(node);
}

auto StatementListNode::GetStatements() const
    -> std::vector<std::shared_ptr<StatementNode>> {
  return statements;
}

auto StatementListNode::GetStartStatementNumber() const -> int {
  return startStatementNumber;
}

auto StatementListNode::GetEndStatementNumber() const -> int {
  return endStatementNumber;
}

void StatementListNode::IncrementStatementNumbers(int value) {
  for (auto i = statements.begin(); i < statements.end(); i++) {
    (*i)->IncrementStatementNumber(value);
  }
  // Stack implementation
  startStatementNumber = statements.back()->GetStatementNumber();
  if (util::instance_of<ContainerStatementNode>(statements.front())) {
    endStatementNumber =
        std::static_pointer_cast<ContainerStatementNode>(statements.front())
            ->GetEndStatementNumber();
  } else {
    endStatementNumber = statements.front()->GetStatementNumber();
  }
}

auto StatementListNode::ToString() const -> std::string {
  std::string str = "stmtLst:\n{\n";
  for (auto i = statements.rbegin(); i < statements.rend(); i++) {
    str += (*i)->ToString();
  }
  str += "}\n";
  return str;
}

void StatementListNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementListNode(
      std::static_pointer_cast<StatementListNode>(currentNode), depth);

  for (auto i = statements.rbegin(); i < statements.rend(); i++) {
    (*i)->AcceptVisitor(*i, extractor, depth + 1);
  }
}
}  // namespace ast
