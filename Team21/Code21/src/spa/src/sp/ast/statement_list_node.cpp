#include "statement_list_node.h"

namespace ast {
void StatementListNode::addStatement(INode *node) {
  statements.push_back(node);
}
}
