#pragma once
#include <string>
#include "named_node.h"
#include "statement_list_node.h"

namespace ast {
class ProcedureNode : public NamedNode {
public:
  ProcedureNode(std::string name, StatementListNode *statements);

  StatementListNode *GetStatements();
  std::ostream &Write(std::ostream &out) const override;

private:
  // Creates some coupling with statement list node
  StatementListNode *statements;
};
}
