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

  void AcceptVisitor(std::shared_ptr<INode> currentNode,
                     std::shared_ptr<design_extractor::Extractor> extractor,
                     int depth) override;

private:
  std::shared_ptr<ProcedureNode> proc;
};
}
