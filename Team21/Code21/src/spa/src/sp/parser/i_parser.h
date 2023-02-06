#pragma once

#include <vector>
#include "sp/ast/ast.h"
#include "token/token.h"

namespace parser {
class IParser {
public:
  virtual ast::AST *Parse(std::vector<token::Token *> input) = 0;
};
}
