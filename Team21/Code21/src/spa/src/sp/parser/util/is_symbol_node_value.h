#pragma once

#include <memory>

#include "sp/ast/astlib.h"

namespace parser {
[[nodiscard]] auto
IsSymbolNodeValue(const std::shared_ptr<ast::INode> &node_ref,
                  std::string_view value) -> bool;

[[nodiscard]] auto
IsSymbolNodeValueAnyOf(const std::shared_ptr<ast::INode> &node_ref,
                       std::initializer_list<std::string_view> values) -> bool;

} // namespace parser
