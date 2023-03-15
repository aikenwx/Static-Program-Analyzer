#pragma once

#include <memory>

#include "statement_node.h"
#include "variable_node.h"

namespace ast {
class PrintNode : public StatementNode {
public:
 explicit PrintNode(std::shared_ptr<VariableNode> var);

 auto GetVariable() -> std::shared_ptr<VariableNode>;
 [[nodiscard]] auto ToString() const -> std::string override;

 void AcceptVisitor(std::shared_ptr<INode> currentNode,
                    std::shared_ptr<design_extractor::Extractor> extractor,
                    int depth) override;

private:
  std::shared_ptr<VariableNode> var;
};
}  // namespace ast
