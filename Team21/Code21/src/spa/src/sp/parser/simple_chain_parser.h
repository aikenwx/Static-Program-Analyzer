#pragma once

#include <string>

#include "context.h"
#include "subparser.h"
#include "i_parser.h"
#include "sp/token/token.h"

namespace parser {
class SimpleChainParser : public IParser {
public:
  SimpleChainParser() noexcept;

  auto Parse(std::vector<std::unique_ptr<token::Token>> input) -> std::unique_ptr<ast::AST> override;

private:
 int statementCounter{};

 std::shared_ptr<Context> context;
 std::shared_ptr<Subparser> subparsers;

 void Shift();
};
}  // namespace parser
