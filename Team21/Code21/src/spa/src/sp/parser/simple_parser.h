#pragma once

#include <string>
#include <vector>
#include "sp/ast/i_node.h"
#include "i_parser.h"
#include "token/token.h"

namespace parser {
class SimpleParser : public IParser {
private:
  std::vector<ast::INode *> stack;
  std::vector<token::Token *>::iterator lookahead;
  
  ast::AST *Parse(std::vector<token::Token *> input) override;
  void Shift();
  void Reduce();
  void Success();
  void Reject();
  bool Check();
};

class EndToken : public token::Token {
public:
  const std::string getValue() override;
};
}
