#include "../ast/procedure_node.h"
#include "proc_relationship.h"

#include <utility>

namespace rel {
auto ProcRelationship::procedureNode() const -> std::shared_ptr<ast::ProcedureNode> { return procedureNode_; }

auto ProcRelationship::procedureName() const -> std::string { return procedureNode_->GetName(); }

auto ProcRelationship::CreateRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) -> std::unique_ptr<ProcRelationship> {
  return std::unique_ptr<ProcRelationship>(new ProcRelationship(std::move(procedureNode)));
};

ProcRelationship::ProcRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) {
  procedureNode_ = procedureNode;
};
}
