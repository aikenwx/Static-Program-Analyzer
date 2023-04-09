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

  const std::unordered_map<std::string, ast::SymbolType> kSymbolToSymbolTypeMap{
      {"&&", ast::SymbolType::kAnd},
      {"=", ast::SymbolType::kAssign},
      {"/", ast::SymbolType::kDivide},
      {"==", ast::SymbolType::kEqual},
      {">=", ast::SymbolType::kGreaterEqual},
      {">", ast::SymbolType::kGreater},
      {"{", ast::SymbolType::kLeftBrace},
      {"(", ast::SymbolType::kLeftParen},
      {"<=", ast::SymbolType::kLesserEqual},
      {"<", ast::SymbolType::kLesser},
      {"-", ast::SymbolType::kMinus},
      {"%", ast::SymbolType::kModulo},
      {"*", ast::SymbolType::kMultiply},
      {"!=", ast::SymbolType::kNotEqual},
      {"!", ast::SymbolType::kNot},
      {"||", ast::SymbolType::kOr},
      {"+", ast::SymbolType::kPlus},
      {")", ast::SymbolType::kRightParen},
      {"}", ast::SymbolType::kRightBrace},
      {";", ast::SymbolType::kSemicolon},
  };
};
} // namespace parser
