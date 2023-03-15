#pragma once

#include <memory>

#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class ReadNode : public StatementNode {
public:
 explicit ReadNode(std::shared_ptr<VariableNode> var);

 auto GetVariable() -> std::shared_ptr<VariableNode>;
 [[nodiscard]] auto ToString() const -> std::string override;

 void AcceptVisitor(const std::shared_ptr<INode>& currentNode,
                    const std::shared_ptr<design_extractor::Extractor>& extractor,
                    int depth) override;
private:
  std::shared_ptr<VariableNode> var;
};
}  // namespace ast
