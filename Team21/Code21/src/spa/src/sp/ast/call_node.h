#pragma once

#include <memory>

#include "identifier_node.h"
#include "name_node.h"
#include "procedure_node.h"
#include "statement_node.h"

namespace ast {
class CallNode : public StatementNode {
public:
  explicit CallNode(const std::shared_ptr<IdentifierNode> &name);

  auto GetProcedureName() -> std::string;
  [[nodiscard]] auto ToString() const -> std::string override;
  auto GetProcedure() -> std::shared_ptr<ProcedureNode>;
  void SetProcedure(const std::shared_ptr<ProcedureNode> &procedure);

  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;

private:
  std::string name_;
  std::weak_ptr<ProcedureNode> procedure_;
};
} // namespace ast
