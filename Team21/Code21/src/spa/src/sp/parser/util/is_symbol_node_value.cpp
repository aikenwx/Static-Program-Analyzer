#include "is_symbol_node_value.h"
#include "util/instance_of.h"
#include <memory>

namespace parser {
[[nodiscard]] auto IsSymbolNodeValue(const std::shared_ptr<ast::INode> &node_ref,
                                     std::string_view value) -> bool {
  return util::instance_of<ast::SymbolNode>(node_ref) &&
         std::static_pointer_cast<ast::SymbolNode>(node_ref)->GetType() == value;
}
} // namespace parser