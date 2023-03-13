#include "sp/design_extractor/cfg_extractor.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "sp/ast/program_node.h"
#include "sp/parser/simple_chain_parser.h"
#include "tokenizer/simple_tokenizer.h"
#include "util/instance_of.h"

namespace test_design_extractor {
std::shared_ptr<ast::ProgramNode> ProgramStringToProgramNode(
    const std::string& program) {
  auto tokenizer = tokenizer::SimpleTokenizer();
  auto tokens = tokenizer.tokenize(program);

  auto parser = parser::SimpleChainParser();
  auto ast = parser.Parse(std::move(tokens));
  std::shared_ptr<ast::INode> root = ast->GetRoot();

  assert(util::instance_of<ast::ProgramNode>(root));

  return std::static_pointer_cast<ast::ProgramNode>(root);
}

SCENARIO(
    "CFGExtractor can handle simple procedures without if/while statements",
    "[sp][sp/design_extractor][sp/design_extractor/cfg_extractor]") {
  GIVEN("A simple program with one procedure") {
    std::string program = R"(
      procedure main {
        a = 1;
        b = 2;
        print x;
        read y;
      })";

    auto programNode = ProgramStringToProgramNode(program);

    WHEN("The CFGExtractor is run on the program") {
      auto extractor = std::make_shared<design_extractor::CFGExtractor>();
      programNode->AcceptVisitor(programNode, extractor, 0);
      auto cfg = extractor->cfg();

      THEN("The CFGExtractor should have extracted a CFG with 1 block") {
        REQUIRE(cfg->Size() == 1);
      }

      THEN(
          "The same block is returned with GetBlockAt() for any valid "
          "statement number") {
        auto block = cfg->GetBlockAt(1);
        REQUIRE(block.has_value());
        REQUIRE(cfg->GetBlockAt(2) == block);
        REQUIRE(cfg->GetBlockAt(3) == block);
        REQUIRE(cfg->GetBlockAt(4) == block);
      }

      THEN("The block has the correct start and end statement numbers") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());

        auto block = block_ptr.value();
        REQUIRE(block->start() == 1);
        REQUIRE(block->end() == 4);
      }

      THEN("The block has no parent blocks") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());

        auto block = block_ptr.value();
        REQUIRE(block->parents().size() == 0);
      }

      THEN("The block has no child blocks") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());

        auto block = block_ptr.value();
        REQUIRE(block->children().size() == 0);
      }
    }
  }
}
}  // namespace test_design_extractor
