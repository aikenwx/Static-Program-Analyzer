#pragma once

#include <memory>

#include "sp/ast/astlib.h"

namespace parser {
  [[nodiscard]] auto IsIdentifierNodeValue(const std::shared_ptr<ast::INode> &node_ref,
                                       std::string_view value) -> bool;
} // namespace parser
