#pragma once

#include <string>
#include <vector>

#include "sp/ast/i_node.h"
#include "i_parser.h"
#include "sp/token/token.h"

namespace parser {
class SimpleParser : public IParser {
public:
  std::unique_ptr<ast::AST> Parse(std::vector<std::unique_ptr<token::Token>> input) override;

private:
  std::vector<std::shared_ptr<ast::INode>> stack;
  std::vector<std::unique_ptr<token::Token>>::iterator lookahead;
  int statementCounter;

  void Shift();
  bool Reduce();
  void Success();
  void Reject();
};
}
