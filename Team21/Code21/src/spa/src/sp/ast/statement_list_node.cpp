#include "statement_list_node.h"

namespace ast {
void StatementListNode::AddStatement(INode *node) {
  statements.push_back(node);
}

std::vector<INode*>* StatementListNode::GetStatements() {
  return &statements;
}

std::string StatementListNode::ToString() const {
  std::string str = "stmtLst:\n{\n";
  for (auto i = statements.rbegin(); i < statements.rend(); i++) {
    str += (*i)->ToString();
  }
  str += "}\n";
  return str;
}
}
