#pragma once

#include "../rel/modifies_stmt_var_relationship.h"
#include "extractor.h"

namespace design_extractor {
class StmtModifiesExtractor : public Extractor {
  // nb: will only handle statement-modifies relns, which means that e.g.:
  // - Modifies(call, v) will not be included here, but should be derived in SP
  //   facade based on Modifies(proc, v)
  // - Modifies(proc, v) will not be included here, but should be derived in SP
  //   facade based on proc stmt numbers and Modifies(stmt, v)
  // - Modifies(container, v) will not be included here, but should be derived
  //   in SP facade based on Parent()
  // - Modifies(read, v) will always hold
  // - Modifies(assg, v) will always hold (if v is on LHS of assg)
 public:
  StmtModifiesExtractor() = default;
  void HandleAssignNode(std::shared_ptr<ast::AssignNode> node,
                        int depth) override;
  void HandleReadNode(std::shared_ptr<ast::ReadNode> node, int depth) override;

  std::vector<std::shared_ptr<rel::ModifiesStmtVarRelationship>>
  GetRelationships();

 private:
  std::vector<std::shared_ptr<rel::ModifiesStmtVarRelationship>> relns_;
};
}  // namespace design_extractor
