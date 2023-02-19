#include <iostream>
#include <string>

#include "catch.hpp"
#include "sp/ast/i_node.h"
#include "sp/ast/program_node.h"
#include "sp/parser/simple_parser.h"
#include "tokenizer/simple_tokenizer.h"
#include "util/instance_of.h"

namespace parser {
tokenizer::SimpleTokenizer tokenizer = tokenizer::SimpleTokenizer();
SimpleParser parser = SimpleParser();

bool CheckRootIsProgram(std::string program) {
  std::vector<std::unique_ptr<token::Token>> tokens =
    tokenizer.tokenize(program);
  std::shared_ptr<ast::INode> root = parser.Parse(std::move(tokens))->GetRoot();
  std::cout << root.get()->ToString() << std::endl;
  return util::instance_of<ast::ProgramNode>(root);
}

TEST_CASE(
    "Parser correctly generates a program node from a valid program with a read statement",
    "[Parser]") {
  std::string program = R"(procedure hello {
    read x;
})";
  REQUIRE(CheckRootIsProgram(program));
};

TEST_CASE(
  "Parser correctly generates a program node from a valid program with a print statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    print x;
})";
  REQUIRE(CheckRootIsProgram(program));
};

TEST_CASE(
  "Parser correctly generates a program node from a valid program with an assigns statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    x = 2;
})";
  REQUIRE(CheckRootIsProgram(program));
};

TEST_CASE(
  "Parser correctly generates a program node from a valid program with an if statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    if (x > 0) then {
      read x;
    } else {
      read x;
    }
})";
  REQUIRE(CheckRootIsProgram(program));
};

TEST_CASE(
  "Parser correctly generates a program node from a valid program with a while statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    while (x > 0) {
      read x;
    }
})";
  REQUIRE(CheckRootIsProgram(program));
};
}  // namespace parser
