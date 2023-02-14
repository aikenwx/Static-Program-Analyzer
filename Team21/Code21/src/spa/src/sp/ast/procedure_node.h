#pragma once
#include <string>
#include "named_node.h"
#include "statement_list_node.h"

namespace ast {
class ProcedureNode : public NamedNode {
public:
  ProcedureNode(std::string name, StatementListNode *statements);

  // Format is a stack
  StatementListNode *GetStatements();
  std::string ToString() const override;

private:
  // Creates some coupling with statement list node
  StatementListNode *statements;
};
}
