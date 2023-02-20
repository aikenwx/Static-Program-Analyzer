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
std::unique_ptr<token::Token> SimpleTokenFactory::CreateToken(std::string value) {
  if (value == "{") {
    return token::LeftBraceToken::CreateToken(value);
  } else if (value == "}") {
    return token::RightBraceToken::CreateToken(value);
  } else if (value == "(") {
    return token::LeftParenToken::CreateToken(value);
  } else if (value == ")") {
    return token::RightParenToken::CreateToken(value);
  } else if (value == ";") {
    return token::SemicolonToken::CreateToken(value);
  } else if (value == "+") {
    return token::PlusToken::CreateToken(value);
  } else if (value == "-") {
    return token::MinusToken::CreateToken(value);
  } else if (value == "*") {
    return token::MultiplyToken::CreateToken(value);
  } else if (value == "/") {
    return token::DivideToken::CreateToken(value);
  } else if (value == "%") {
    return token::ModuloToken::CreateToken(value);
  } else if (value == "==") {
    return token::EqualToken::CreateToken(value);
  } else if (value == "!=") {
    return token::NotEqualToken::CreateToken(value);
  } else if (value == "<") {
    return token::LessThanToken::CreateToken(value);
  } else if (value == "<=") {
    return token::LessEqualToken::CreateToken(value);
  } else if (value == ">") {
    return token::GreaterThanToken::CreateToken(value);
  } else if (value == ">=") {
    return token::GreaterEqualToken::CreateToken(value);
  } else if (value == "!") {
    return token::NotToken::CreateToken(value);
  } else if (value == "=") {
    return token::AssignToken::CreateToken(value);
  } else if (value == "&&") {
    return token::AndToken::CreateToken(value);
  } else if (value == "||") {
    return token::OrToken::CreateToken(value);
  } else if (util::is_integer(value)) {
    return token::IntegerToken::CreateToken(value);
  } else {
    return token::IdentifierToken::CreateToken(value);
  }
};

bool SimpleTokenFactory::isWhitespace(const char& c) {
  return (kWhitespaceTokens.find(c) != kWhitespaceTokens.end());
}

bool SimpleTokenFactory::isSymbolPrefixChar(const char& c) {
  return (kSymbolTokens.find(c) != kSymbolTokens.end());
}

CheckSymbolResult SimpleTokenFactory::checkSymbol(const std::string& value) {
  auto it = kSymbolTokens.find(value[0]);
  if (it == kSymbolTokens.end()) {
    return {false, false};
  }
  std::vector<std::string> candidates = it->second;
  bool isValidPrefix = false;
  for (std::string candidate : candidates) {
    if (candidate.size() < value.size()) {
      continue;
    }

    auto res = std::mismatch(value.begin(), value.end(), candidate.begin());
    if (res.first == value.end()) { // prefix match
      isValidPrefix = true;
      if (res.second == candidate.end()) { // exact match
        return {isValidPrefix, true}; // {isValidPrefix, isValidSymbol}
      }
    }
  }
  return {isValidPrefix, false};
}

SimpleTokenFactory::SimpleTokenFactory() {}

const std::unordered_map<char, std::vector<std::string>>
    SimpleTokenFactory::kSymbolTokens{
        {'+', {"+"}}, {'-', {"-"}}, {'*', {"*"}},
        {'/', {"/"}},       {'%', {"%"}},       {'(', {"("}},
        {')', {")"}},       {'{', {"{"}},       {'}', {"}"}},
        {';', {";"}},       {'=', {"==", "="}}, {'!', {"!=", "!"}},
        {'<', {"<=", "<"}}, {'>', {">=", ">"}}, {'&', {"&&"}},
        {'|', {"||"}},
    };

const std::unordered_set<char> SimpleTokenFactory::kWhitespaceTokens{' ', '\t',
                                                                    '\n', '\r'};
}  // namespace token_factory
