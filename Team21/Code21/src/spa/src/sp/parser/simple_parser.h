#pragma once
#include <string>
#include <vector>
#include "sp/ast/i_node.h"
#include "i_parser.h"
#include "token/token.h"

namespace parser {
class SimpleParser : public IParser {
public:
  ast::AST *Parse(std::vector<token::Token *> input) override;

private:
  std::vector<ast::INode *> stack;
  std::vector<token::Token *>::iterator lookahead;
  int statementCounter;

  void Shift();
  void Reduce();
  void Success();
  void Reject();
  bool Check();
};
}
