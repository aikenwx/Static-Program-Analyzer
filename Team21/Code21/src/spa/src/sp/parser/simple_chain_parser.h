#pragma once

#include <string>
#include <unordered_map>

#include "context.h"
#include "i_parser.h"
#include "sp/ast/symbol_node.h"
#include "sp/token/token.h"
#include "subparser.h"

namespace parser {
class SimpleChainParser : public IParser {
public:
  SimpleChainParser() noexcept;

  auto Parse(std::vector<std::unique_ptr<token::Token>> input)
      -> std::pair<bool, std::unique_ptr<ast::AST>> override;

private:
  int statementCounter{};

  std::shared_ptr<Context> context;
  std::shared_ptr<Subparser> subparsers;

  void Shift();
};
} // namespace parser
