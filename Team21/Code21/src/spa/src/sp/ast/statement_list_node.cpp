#include "statement_list_node.h"

void StatementListNode::addStatement(INode *node) {
  statements.push_back(node);
}
