//
// Created by Aiken Wong on 3/4/23.
//

#include "ParentStatement.h"

auto ParentStatement::isParentStatement(Entity* entity) -> bool {
  return dynamic_cast<ParentStatement*>(entity) != nullptr;
}

ParentStatement::ParentStatement(const EntityType* entityType, int statementNumber) : Statement(entityType, statementNumber) {}
