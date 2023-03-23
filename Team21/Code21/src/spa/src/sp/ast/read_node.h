#pragma once

#include <memory>

#include "identifier_node.h"
#include "statement_node.h"

namespace ast {
class ReadNode : public StatementNode {
public:
  explicit ReadNode(std::shared_ptr<IdentifierNode> var);

  auto GetVariable() -> std::shared_ptr<IdentifierNode>;
  [[nodiscard]] auto ToString() const -> std::string override;

  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;

private:
  std::shared_ptr<IdentifierNode> var;
};
} // namespace ast
