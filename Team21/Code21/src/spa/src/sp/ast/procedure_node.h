#pragma once

#include <memory>
#include <string>

#include "named_node.h"
#include "statement_list_node.h"

namespace ast {
class ProcedureNode : public NamedNode {
public:
  ProcedureNode(std::string name, std::shared_ptr<StatementListNode> statements);

  // Format is a stack
  std::shared_ptr<StatementListNode> GetStatements();
  std::string ToString() const override;

private:
  // Creates some coupling with statement list node
  std::shared_ptr<StatementListNode> statements;
};
}
