#include "statement_list_node.h"

namespace ast {
void StatementListNode::AddStatement(INode *node) {
  statements.push_back(node);
}

std::vector<INode*>* StatementListNode::GetStatements() {
  return &statements;
}

std::ostream &StatementListNode::Write(std::ostream &out) const {
  out << "stmtLst:" << "\n{";
  for (auto i = statements.rbegin(); i < statements.rend(); i++) {
    (*i)->Write(out);
  }
  out << "}" << "\n";
  return out;
}
}
