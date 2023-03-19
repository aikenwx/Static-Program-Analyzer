#pragma once

#include <memory>
#include <vector>

#include "procedure_node.h"

namespace ast {
class ProgramNode : public INode {
public:
  void AddProcedure(const std::shared_ptr<ProcedureNode> &procedure);
  [[nodiscard]] auto GetProcedures() const
      -> std::vector<std::shared_ptr<ProcedureNode>>;
  [[nodiscard]] auto ToString() const -> std::string override;
  auto GetTotalStatementCount() -> int;
  auto ContainsProcedure(std::string const &procedureName) -> bool;
  auto GetProcedure(std::string const &procedureName)
      -> std::shared_ptr<ProcedureNode>;

  void AcceptVisitor(design_extractor::Extractor &extractor,
                     int depth) override;

private:
  std::vector<std::shared_ptr<ProcedureNode>> procedures;
};
} // namespace ast
