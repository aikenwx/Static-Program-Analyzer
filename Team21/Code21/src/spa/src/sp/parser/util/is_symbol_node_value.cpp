#include "is_symbol_node_value.h"

#include <algorithm>
#include <memory>

#include "sp/ast/symbol_node.h"
#include "util/instance_of.h"

namespace parser {
[[nodiscard]] auto IsSymbolNodeValue(const std::shared_ptr<ast::INode> &node_ref,
                                     std::string_view value) -> bool {
  return util::instance_of<ast::SymbolNode>(node_ref) &&
         std::static_pointer_cast<ast::SymbolNode>(node_ref)->GetType() == value;
}

[[nodiscard]] auto IsSymbolNodeValueAnyOf(
    const std::shared_ptr<ast::INode> &node_ref,
    std::initializer_list<std::string_view> values) -> bool {
  if (!util::instance_of<ast::SymbolNode>(node_ref)) {
    return false;
  };

  auto const &node = std::static_pointer_cast<ast::SymbolNode>(node_ref);

  return std::any_of(values.begin(), values.end(), [node](auto value) {
    return node->GetType() ==
           value;
  });
}
} // namespace parser
