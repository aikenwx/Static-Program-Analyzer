#include <iostream>
#include <string>
#include "catch.hpp"
#include "sp/ast/i_node.h"
#include "sp/ast/program_node.h"
#include "sp/parser/simple_parser.h"
#include "tokenizer/simple_tokenizer.h"
#include "util/instance_of.h"


namespace parser {
TEST_CASE("Parser correctly generates a program node from a simple, valid program", "[Parser]") {
  std::string program = R"(procedure hello {
    read x;
})";
  tokenizer::SimpleTokenizer* tokenizer =
    tokenizer::SimpleTokenizer::getInstance();
  std::vector<token::Token*> tokens = tokenizer->tokenize(program);
  IParser *parser = new SimpleParser();
  ast::INode *root = parser->Parse(tokens)->GetRoot();
  REQUIRE(util::instance_of<ast::ProgramNode>(root));
};
}
