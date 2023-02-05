#include <iostream>

#include "catch.hpp"
#include "tokenizer/simple_tokenizer.h"

namespace tokenizer {
TEST_CASE("Tokenizer can tokenize a simple program", "[tokenizer]") {
  std::string program = R"(procedure main {
    flag = 0;
    call computeCentroid;
    call printResults;
}
procedure readPoint {
    read x;
    read y;
}
procedure printResults {
    print flag;
    print cenX;
    print cenY;
    print normSq;
}
procedure computeCentroid {
    count = 0;
    cenX = 0;
    cenY = 0;
    call readPoint;
    while ((x != 0) && (y != 0)) {
        count = count + 1;
        cenX = cenX + x;
        cenY = cenY + y;
        call readPoint;
    }
    if (count == 0) then {
        flag = 1;
    } else {
        cenX = cenX / count;
        cenY = cenY / count;
    }
    normSq = cenX * cenX + cenY * cenY;
})";

  std::vector<std::string> expected{
      "procedure",
      "main",
      "{",
      "flag",
      "=",
      "0",
      ";",
      "call",
      "computeCentroid",
      ";",
      "call",
      "printResults",
      ";",
      "}",
      "procedure",
      "readPoint",
      "{",
      "read",
      "x",
      ";",
      "read",
      "y",
      ";",
      "}",
      "procedure",
      "printResults",
      "{",
      "print",
      "flag",
      ";",
      "print",
      "cenX",
      ";",
      "print",
      "cenY",
      ";",
      "print",
      "normSq",
      ";",
      "}",
      "procedure",
      "computeCentroid",
      "{",
      "count",
      "=",
      "0",
      ";",
      "cenX",
      "=",
      "0",
      ";",
      "cenY",
      "=",
      "0",
      ";",
      "call",
      "readPoint",
      ";",
      "while",
      "(",
      "(",
      "x",
      "!=",
      "0",
      ")",
      "&&",
      "(",
      "y",
      "!=",
      "0",
      ")",
      ")",
      "{",
      "count",
      "=",
      "count",
      "+",
      "1",
      ";",
      "cenX",
      "=",
      "cenX",
      "+",
      "x",
      ";",
      "cenY",
      "=",
      "cenY",
      "+",
      "y",
      ";",
      "call",
      "readPoint",
      ";",
      "}",
      "if",
      "(",
      "count",
      "==",
      "0",
      ")",
      "then",
      "{",
      "flag",
      "=",
      "1",
      ";",
      "}",
      "else",
      "{",
      "cenX",
      "=",
      "cenX",
      "/",
      "count",
      ";",
      "cenY",
      "=",
      "cenY",
      "/",
      "count",
      ";",
      "}",
      "normSq",
      "=",
      "cenX",
      "*",
      "cenX",
      "+",
      "cenY",
      "*",
      "cenY",
      ";",
  };
  tokenizer::SimpleTokenizer* tokenizer =
      tokenizer::SimpleTokenizer::getInstance();

  std::vector<token::Token*> tokens = tokenizer->tokenize(program);
  auto mapTokenToString = [](token::Token* token) { return token->getValue(); };
  std::vector<std::string> tokenStrings(tokens.size());
  std::transform(tokens.begin(), tokens.end(), tokenStrings.begin(),
                 [](token::Token* t) { return t->getValue(); });

  REQUIRE(tokenStrings == expected);
};
}  // namespace tokenizer
