#pragma once

#include <vector>
#include "sp/ast/ast.h"
#include "token/token.h"

class IParser {
public:
  virtual AST *parse(std::vector<token::Token *> input) = 0;
};
