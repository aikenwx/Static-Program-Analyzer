#pragma once
#include <string>
#include "named_node.h"
#include "statement_list_node.h"

class ProcedureNode : public NamedNode {
public:
  ProcedureNode(std::string name, StatementListNode *statements);

  StatementListNode *getStatements();

private:
  // Creates some coupling with statement list node
  StatementListNode *statements;
};
