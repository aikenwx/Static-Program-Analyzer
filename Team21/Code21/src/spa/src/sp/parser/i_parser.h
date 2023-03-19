#pragma once

#include <memory>
#include <vector>

#include "sp/ast/ast.h"
#include "sp/token/token.h"

namespace parser {
class IParser {
public:
  IParser() = default;
  IParser(const IParser &) = delete;
  IParser(IParser &&) = delete;
  auto operator=(const IParser &) -> IParser & = delete;
  auto operator=(IParser &&) -> IParser & = delete;
  virtual ~IParser() = default;
  virtual auto Parse(std::vector<std::unique_ptr<token::Token>> input) -> std::unique_ptr<ast::AST> = 0;
};
}  // namespace parser
