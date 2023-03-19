#include "simple_token_factory.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include "../token/and_token.h"
#include "../token/assign_token.h"
#include "../token/divide_token.h"
#include "../token/end_token.h"
#include "../token/equal_token.h"
#include "../token/greater_equal_token.h"
#include "../token/greater_than_token.h"
#include "../token/identifier_token.h"
#include "../token/integer_token.h"
#include "../token/left_brace_token.h"
#include "../token/left_paren_token.h"
#include "../token/less_equal_token.h"
#include "../token/less_than_token.h"
#include "../token/minus_token.h"
#include "../token/modulo_token.h"
#include "../token/multiply_token.h"
#include "../token/not_equal_token.h"
#include "../token/not_token.h"
#include "../token/or_token.h"
#include "../token/plus_token.h"
#include "../token/right_brace_token.h"
#include "../token/right_paren_token.h"
#include "../token/semicolon_token.h"
#include "../token/token.h"
#include "util/is_integer.h"

namespace token_factory {
auto SimpleTokenFactory::CreateToken(std::string_view value)
    -> std::unique_ptr<token::Token> {
  if (value == "{") {
    return token::LeftBraceToken::CreateToken(value);
  }
  if (value == "}") {
    return token::RightBraceToken::CreateToken(value);
  }
  if (value == "(") {
    return token::LeftParenToken::CreateToken(value);
  }
  if (value == ")") {
    return token::RightParenToken::CreateToken(value);
  }
  if (value == ";") {
    return token::SemicolonToken::CreateToken(value);
  }
  if (value == "+") {
    return token::PlusToken::CreateToken(value);
  }
  if (value == "-") {
    return token::MinusToken::CreateToken(value);
  }
  if (value == "*") {
    return token::MultiplyToken::CreateToken(value);
  }
  if (value == "/") {
    return token::DivideToken::CreateToken(value);
  }
  if (value == "%") {
    return token::ModuloToken::CreateToken(value);
  }
  if (value == "==") {
    return token::EqualToken::CreateToken(value);
  }
  if (value == "!=") {
    return token::NotEqualToken::CreateToken(value);
  }
  if (value == "<") {
    return token::LessThanToken::CreateToken(value);
  }
  if (value == "<=") {
    return token::LessEqualToken::CreateToken(value);
  }
  if (value == ">") {
    return token::GreaterThanToken::CreateToken(value);
  }
  if (value == ">=") {
    return token::GreaterEqualToken::CreateToken(value);
  }
  if (value == "!") {
    return token::NotToken::CreateToken(value);
  }
  if (value == "=") {
    return token::AssignToken::CreateToken(value);
  }
  if (value == "&&") {
    return token::AndToken::CreateToken(value);
  }
  if (value == "||") {
    return token::OrToken::CreateToken(value);
  }
  if (util::is_integer(value)) {
    return token::IntegerToken::CreateToken(value);
  }
  return token::IdentifierToken::CreateToken(value);
};

auto SimpleTokenFactory::isWhitespace(const char &chr) -> bool {
  return kWhitespaceTokens.find(chr) != kWhitespaceTokens.end();
}

auto SimpleTokenFactory::isSymbolPrefixChar(const char &chr) -> bool {
  return kSymbolTokens.find(chr) != kSymbolTokens.end();
}

auto SimpleTokenFactory::checkSymbol(std::string_view value)
    -> CheckSymbolResult {
  auto iter = kSymbolTokens.find(value[0]);
  if (iter == kSymbolTokens.end()) {
    return {false, false};
  }
  std::vector<std::string> candidates = iter->second;
  bool isValidPrefix = false;
  for (std::string candidate : candidates) {
    if (candidate.size() < value.size()) {
      continue;
    }

    auto [first, second] =
        std::mismatch(value.begin(), value.end(), candidate.begin());
    if (first == value.end()) { // prefix match
      isValidPrefix = true;
      if (second == candidate.end()) { // exact match
        return {isValidPrefix, true};
      }
    }
  }
  return {isValidPrefix, false};
}

const std::unordered_map<char, std::vector<std::string>>
    SimpleTokenFactory::kSymbolTokens{
        {'+', {"+"}},       {'-', {"-"}},       {'*', {"*"}},
        {'/', {"/"}},       {'%', {"%"}},       {'(', {"("}},
        {')', {")"}},       {'{', {"{"}},       {'}', {"}"}},
        {';', {";"}},       {'=', {"==", "="}}, {'!', {"!=", "!"}},
        {'<', {"<=", "<"}}, {'>', {">=", ">"}}, {'&', {"&&"}},
        {'|', {"||"}},
    };

const std::unordered_set<char> SimpleTokenFactory::kWhitespaceTokens{
    ' ', '\t', '\n', '\r'};
} // namespace token_factory
