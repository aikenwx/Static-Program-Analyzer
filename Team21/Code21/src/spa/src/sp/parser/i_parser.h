#pragma once

#include <vector>
#include "sp/ast/AST.h"
#include "token/Token.h"

class IParser {
public:
  virtual AST *parse(std::vector<token::Token *> input) = 0;
};
