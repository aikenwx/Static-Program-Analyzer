#include "util.h"

#include "sp/ast/print_node.h"
#include "sp/ast/read_node.h"
#include "util/instance_of.h"

namespace rel {
EntityType resolveEntityType(ast::StatementNode* statementNode) {
  if (util::instance_of<ast::PrintNode>(statementNode)) {
    return EntityType::PRINT_STATEMENT;
  } else if (util::instance_of<ast::ReadNode>(statementNode)) {
    return EntityType::READ_STATEMENT;
  } else {
    throw std::runtime_error("Invalid statement node type");
  }
};
}  // namespace rel
