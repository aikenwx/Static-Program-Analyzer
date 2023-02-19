#pragma once

#include <memory>

#include "statement_node.h"
#include "procedure_node.h"

namespace ast {
class CallNode : public StatementNode {
public:
  CallNode(std::shared_ptr<ProcedureNode> var);

  std::shared_ptr<ProcedureNode> GetProcedure();
  std::string ToString() const override;

private:
  std::shared_ptr<Procedure> proc;
};
}
