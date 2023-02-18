#include <iostream>

#include "catch.hpp"
#include "common.h"
#include "tokenizer/simple_tokenizer.h"

namespace tokenizer {
std::vector<std::string> expectedTokenStrings{
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
    "}",
};
TEST_CASE("Tokenizer can tokenize a simple, well-formatted program",
          "[tokenizer]") {
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

  tokenizer::SimpleTokenizer tokenizer =
      tokenizer::SimpleTokenizer();

  std::vector<std::unique_ptr<token::Token>> tokens = tokenizer.tokenize(program);

  REQUIRE(verifyTokenizerOutput(tokens, expectedTokenStrings));
};

TEST_CASE("Tokenizer can tokenize a simple, minified program", "[tokenizer]") {
  std::string program =
      "procedure main{flag=0;call computeCentroid;call printResults;}procedure "
      "readPoint{read x;read y;}procedure printResults{print flag;print "
      "cenX;print cenY;print normSq;}procedure "
      "computeCentroid{count=0;cenX=0;cenY=0;call "
      "readPoint;while((x!=0)&&(y!=0)){count=count+1;cenX=cenX+x;cenY=cenY+y;"
      "call "
      "readPoint;}if(count==0)then{flag=1;}else{cenX=cenX/count;cenY=cenY/"
      "count;}normSq=cenX*cenX+cenY*cenY;}";

  tokenizer::SimpleTokenizer tokenizer =
      tokenizer::SimpleTokenizer();

  std::vector<std::unique_ptr<token::Token>> tokens = tokenizer.tokenize(program);

  REQUIRE(verifyTokenizerOutput(tokens, expectedTokenStrings));
};
}  // namespace tokenizer
