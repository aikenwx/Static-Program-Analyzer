#pragma once

#include "sp/ast/statement_node.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace rel {
  EntityType resolveEntityType(ast::StatementNode* statementNode);
}
