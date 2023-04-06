#include "simple_token_factory.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../token/identifier_token.h"
#include "../token/integer_token.h"
#include "../token/symbol_token.h"
#include "../token/token.h"
#include "util/is_integer.h"

namespace token_factory {
auto SimpleTokenFactory::CreateToken(const std::string& value)
    -> std::unique_ptr<token::Token> {
  if (kValidSymbols.find(value) != kValidSymbols.end()) {
    return token::SymbolToken::CreateToken(value);
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

const std::unordered_set<std::string> SimpleTokenFactory::kValidSymbols{
    "+",  "-", "*",  "/", "%",  "(", ")",  "{", "}",  ";",
    "==", "=", "!=", "!", "<=", "<", ">=", ">", "&&", "||"};

const std::unordered_set<char> SimpleTokenFactory::kWhitespaceTokens{
    ' ', '\t', '\n', '\r'};
} // namespace token_factory
