#include "query_preprocess/query_tokenizer.h"
#include "query/query_exceptions.h"

#include <cctype>
#include <set>
#include <utility>

namespace qps {

const std::set<std::string> special_relationship = {"Follows", "Parent", "Calls", "Next", "Affects"};
const std::set<char> single_tokens{'(', ')', '*', ',', ';', '.', '_', '<', '>', '='};

QueryTokenizer::QueryTokenizer(std::string source)
    : queryString{std::move(source)}, currentIndex{0} {}

auto QueryTokenizer::peek() -> char {
  char nextChar{};
  if (currentIndex < queryString.length()) {
    nextChar = queryString[currentIndex];
  } else {
    nextChar = EOF;
  }
  return nextChar;
}

auto QueryTokenizer::next() -> char {
  char nextChar{peek()};
  currentIndex++;
  return nextChar;
}

auto QueryTokenizer::isEnd() -> bool {
  return currentIndex == queryString.length();
}

void QueryTokenizer::readPhrase() {
  while (isalnum(peek()) != 0) {
    currentString += next();
  }
  //Deals with special relationship with extra single token *
  if ((special_relationship.count(currentString) != 0U) && peek() == '*') {
    currentString += next();
  }
  //Deals with special attrName: stmt#
  if (currentString == "stmt" && peek() == '#') {
    currentString += next();
  }
}

void QueryTokenizer::readNumber() {
  while (isdigit(peek()) != 0) {
    currentString += next();
  }
}

auto QueryTokenizer::tokenize() -> std::vector<std::string> {
  while (currentIndex < queryString.length()) {
    if (peek() == EOF) {
      break;
    }
    if (isalpha(peek()) != 0) {
      readPhrase();
      tokens.push_back(currentString);
    } else if (isdigit(peek()) != 0) {
      readNumber();
      tokens.push_back(currentString);
    } else if (peek() == '"') {
      next();
      tokens.emplace_back("\"");
      while (peek() != '"' && !isEnd()) {
        currentString += next();
      }
      tokens.push_back(currentString);
      if (peek() == '"') {
        next();
        tokens.emplace_back("\"");
      }
    } else if (isspace(peek()) != 0) {
      next();
    } else if (single_tokens.count(peek()) != 0U) {
      currentString += next();
      tokens.push_back(currentString);
    } else {
      throw QueryException(ErrorType::Syntactic, "Unexpected character in query string: " + std::string(1, peek()));
    }
    currentString = "";
  }
  return tokens;
}

}  // namespace qps
