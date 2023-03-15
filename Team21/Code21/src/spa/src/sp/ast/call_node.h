#pragma once

#include <memory>

#include "procedure_node.h"
#include "statement_node.h"
#include "name_node.h"

namespace ast {
class CallNode : public StatementNode {
public:
 explicit CallNode(const std::shared_ptr<NameNode>& name);

 auto GetProcedureName() -> std::string;
 [[nodiscard]] auto ToString() const -> std::string override;
 auto GetProcedure() -> std::shared_ptr<ProcedureNode>;
 void SetProcedure(const std::shared_ptr<ProcedureNode>& procedure);

 void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                    const std::shared_ptr<design_extractor::Extractor>& extractor,
                    int depth) override;

private:
  std::string name_;
  std::weak_ptr<ProcedureNode> procedure_;
};
}  // namespace ast
