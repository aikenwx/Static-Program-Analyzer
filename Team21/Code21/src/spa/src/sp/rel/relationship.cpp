#pragma once

#include "relationship.h"

namespace rel {
int StmtRelationship::statementNumber() {
  return statementNode_->GetStatementNumber();
};
}  // namespace rel
