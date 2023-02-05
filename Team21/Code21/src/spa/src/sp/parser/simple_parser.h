#pragma once

#include <string>
#include <vector>
#include "sp/ast/i_node.h"
#include "i_parser.h"
#include "token/token.h"

class SimpleParser : public IParser {
private:
  std::vector<INode *> stack;
  std::vector<token::Token *>::iterator lookahead;

  AST *parse(std::vector<token::Token *> input) override;
  void shift();
  void reduce();
  void success();
  void reject();
  bool check();
};

class EndToken : public token::Token {
public:
  const std::string getValue() override;
};
