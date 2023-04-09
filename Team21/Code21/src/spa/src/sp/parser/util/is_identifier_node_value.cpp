#include "is_identifier_node_value.h"

#include <memory>

#include "sp/ast/identifier_node.h"
#include "util/instance_of.h"

namespace parser {
[[nodiscard]] auto IsIdentifierNodeValue(const std::shared_ptr<ast::INode> &node_ref,
                                     std::string_view value) -> bool {
  return util::instance_of<ast::IdentifierNode>(node_ref) &&
         std::static_pointer_cast<ast::IdentifierNode>(node_ref)->GetValue() == value;
}
} // namespace parser