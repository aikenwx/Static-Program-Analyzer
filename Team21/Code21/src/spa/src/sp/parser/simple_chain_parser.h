#pragma once

#include <string>

#include "context.h"
#include "subparser.h"
#include "i_parser.h"
#include "token/token.h"

namespace parser {
class SimpleChainParser : public IParser {
public:
  SimpleChainParser();

  std::unique_ptr<ast::AST> Parse(std::vector<std::unique_ptr<token::Token>> input) override;

private:
  int statementCounter;

  std::shared_ptr<Context> context;
  std::shared_ptr<Subparser> subparsers;

  void Shift();
  //bool Reduce();
  //void Success();
  //void Reject();
};
}
