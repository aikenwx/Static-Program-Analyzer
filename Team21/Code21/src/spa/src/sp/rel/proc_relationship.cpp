#include "../ast/procedure_node.h"
#include "proc_relationship.h"

#include <utility>

namespace rel {
std::shared_ptr<ast::ProcedureNode> ProcRelationship::procedureNode() const { return procedureNode_; }

std::string ProcRelationship::procedureName() const { return procedureNode_->GetName(); }

std::unique_ptr<ProcRelationship> ProcRelationship::CreateRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) {
  return std::unique_ptr<ProcRelationship>(new ProcRelationship(std::move(procedureNode)));
};

ProcRelationship::ProcRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) {
  procedureNode_ = procedureNode;
};
}
