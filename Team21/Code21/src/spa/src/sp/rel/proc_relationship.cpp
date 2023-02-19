#include "proc_relationship.h"

namespace rel {
std::shared_ptr<ast::ProcedureNode> ProcRelationship::procedureNode() { return procedureNode_; }

std::string ProcRelationship::procedureName() { return procedureNode_->GetName(); }

std::unique_ptr<ProcRelationship> ProcRelationship::CreateRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) {
  return std::unique_ptr<ProcRelationship>(new ProcRelationship(procedureNode));
};

ProcRelationship::ProcRelationship(std::shared_ptr<ast::ProcedureNode> procedureNode) {
  procedureNode_ = procedureNode;
};
}
