#pragma once

#include <memory>
#include <vector>

#include "sp/ast/ast.h"
#include "sp/token/token.h"

namespace parser {
class IParser {
public:
  virtual std::unique_ptr<ast::AST> Parse(std::vector<std::unique_ptr<token::Token>> input) = 0;
};
}
