#include <iostream>
#include <string>

#include "catch.hpp"
#include "sp/ast/i_node.h"
#include "sp/ast/program_node.h"
#include "sp/parser/simple_parser.h"
#include "tokenizer/simple_tokenizer.h"
#include "util/instance_of.h"

namespace parser {
TEST_CASE(
    "Parser correctly generates a program node from a simple, valid program",
    "[Parser]") {
  std::string program = R"(procedure hello {
    read x;
}
procedure world {
    print y;
}
procedure print {
    print read;
    read procedure;
})";
  tokenizer::SimpleTokenizer tokenizer = tokenizer::SimpleTokenizer();
  std::vector<std::unique_ptr<token::Token>> tokens =
      tokenizer.tokenize(program);
  SimpleParser parser = SimpleParser();
  std::shared_ptr<ast::INode> root = parser.Parse(std::move(tokens))->GetRoot();
  REQUIRE(util::instance_of<ast::ProgramNode>(root));
};
}  // namespace parser
