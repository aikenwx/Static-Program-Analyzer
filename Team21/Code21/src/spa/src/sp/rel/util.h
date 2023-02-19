#pragma once

#include <memory>

#include "sp/ast/statement_node.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace rel {
  EntityType resolveEntityType(std::shared_ptr<ast::StatementNode> statementNode);
}
