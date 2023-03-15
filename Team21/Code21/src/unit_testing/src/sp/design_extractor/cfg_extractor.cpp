#include "sp/design_extractor/cfg_extractor.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <unordered_set>

#include "sp/ast/program_node.h"
#include "sp/parser/simple_chain_parser.h"
#include "tokenizer/simple_tokenizer.h"
#include "util/instance_of.h"

namespace test_design_extractor {
auto ProgramStringToProgramNode(const std::string& program)
    -> std::shared_ptr<ast::ProgramNode> {
  auto tokenizer = tokenizer::SimpleTokenizer();
  auto tokens = tokenizer.tokenize(program);

  auto parser = parser::SimpleChainParser();
  auto ast = parser.Parse(std::move(tokens));
  std::shared_ptr<ast::INode> root = ast->GetRoot();

  assert(util::instance_of<ast::ProgramNode>(root)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

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
        REQUIRE(block->parents().empty());
      }

      THEN("The block has no child blocks") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());

        auto block = block_ptr.value();
        REQUIRE(block->children().empty());
      }
    }
  }

  GIVEN("A simple program with multiple procedures") {
    std::string program = R"(
      procedure main {
        a = 1;
        b = 2;
        print x;
        read y;
      }

      procedure foo {
        a = 1;
        b = 2;
        print x;
        read y;
      }

      procedure bar {
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

      THEN("The CFGExtractor should have extracted a CFG with 3 blocks") {
        REQUIRE(cfg->Size() == 3);
      }

      THEN(
          "The correct blocks are returned with GetBlockAt() for any valid "
          "statement number") {
        auto block = cfg->GetBlockAt(1);
        REQUIRE(block.has_value());
        REQUIRE(cfg->GetBlockAt(2) == block);
        REQUIRE(cfg->GetBlockAt(3) == block);
        REQUIRE(cfg->GetBlockAt(4) == block);

        block = cfg->GetBlockAt(5);
        REQUIRE(block.has_value());
        REQUIRE(cfg->GetBlockAt(6) == block);
        REQUIRE(cfg->GetBlockAt(7) == block);
        REQUIRE(cfg->GetBlockAt(8) == block);

        block = cfg->GetBlockAt(9);
        REQUIRE(block.has_value());
        REQUIRE(cfg->GetBlockAt(10) == block);
        REQUIRE(cfg->GetBlockAt(11) == block);
        REQUIRE(cfg->GetBlockAt(12) == block);
      }

      THEN("The blocks have the correct start and end statement numbers") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());

        auto block = block_ptr.value();
        REQUIRE(block->start() == 1);
        REQUIRE(block->end() == 4);

        block_ptr = cfg->GetBlockAt(5);
        REQUIRE(block_ptr.has_value());

        block = block_ptr.value();
        REQUIRE(block->start() == 5);
        REQUIRE(block->end() == 8);

        block_ptr = cfg->GetBlockAt(9);
        REQUIRE(block_ptr.has_value());

        block = block_ptr.value();
        REQUIRE(block->start() == 9);
        REQUIRE(block->end() == 12);
      }

      THEN("The blocks do not have any parent blocks") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());

        auto block = block_ptr.value();
        REQUIRE(block->parents().empty());

        block_ptr = cfg->GetBlockAt(5);
        REQUIRE(block_ptr.has_value());

        block = block_ptr.value();
        REQUIRE(block->parents().empty());

        block_ptr = cfg->GetBlockAt(9);
        REQUIRE(block_ptr.has_value());

        block = block_ptr.value();
        REQUIRE(block->parents().empty());
      }

      THEN("The blocks do not have any child blocks") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());

        auto block = block_ptr.value();
        REQUIRE(block->children().empty());

        block_ptr = cfg->GetBlockAt(5);
        REQUIRE(block_ptr.has_value());

        block = block_ptr.value();
        REQUIRE(block->children().empty());

        block_ptr = cfg->GetBlockAt(9);
        REQUIRE(block_ptr.has_value());

        block = block_ptr.value();
        REQUIRE(block->children().empty());
      }
    }
  }
}

SCENARIO(
    "CFGExtractor can handle procedures containing only one container "
    "statement",
    "[sp][sp/design_extractor][sp/design_extractor/cfg_extractor]") {
  GIVEN("A simple program with one procedure") {
    std::string program = R"(
      procedure main {
        if (a == 1) then {
          a = 1;
          b = 2;
          print x;
          read y;
        } else {
          c = 3;
          d = 4;
          print a;
          print b;
        }
      })";

    auto programNode = ProgramStringToProgramNode(program);

    WHEN("The CFGExtractor is run on the program") {
      auto extractor = std::make_shared<design_extractor::CFGExtractor>();
      programNode->AcceptVisitor(programNode, extractor, 0);
      auto cfg = extractor->cfg();

      THEN("The CFGExtractor should have extracted a CFG with 3 blocks") {
        REQUIRE(cfg->Size() == 3);
      }

      THEN(
          "The correct blocks are returned with GetBlockAt() for any valid "
          "statement number") {
        auto condBlock = cfg->GetBlockAt(1);
        REQUIRE(condBlock.has_value());

        auto thenBlock = cfg->GetBlockAt(2);
        REQUIRE(thenBlock.has_value());
        REQUIRE(condBlock != thenBlock);

        auto elseBlock = cfg->GetBlockAt(6);
        REQUIRE(elseBlock.has_value());
        REQUIRE(condBlock != elseBlock);
        REQUIRE(thenBlock != elseBlock);

        REQUIRE(cfg->GetBlockAt(3) == thenBlock);
        REQUIRE(cfg->GetBlockAt(4) == thenBlock);
        REQUIRE(cfg->GetBlockAt(5) == thenBlock);

        REQUIRE(cfg->GetBlockAt(7) == elseBlock);
        REQUIRE(cfg->GetBlockAt(8) == elseBlock);
        REQUIRE(cfg->GetBlockAt(9) == elseBlock);
      }

      // at this point we should've validated that the blocks exist
      auto condBlock = cfg->GetBlockAt(1).value();
      auto thenBlock = cfg->GetBlockAt(2).value();
      auto elseBlock = cfg->GetBlockAt(6).value();

      THEN("The blocks have the correct start and end statement numbers") {
        REQUIRE(condBlock->start() == 1);
        REQUIRE(condBlock->end() == 1);

        REQUIRE(thenBlock->start() == 2);
        REQUIRE(thenBlock->end() == 5);

        REQUIRE(elseBlock->start() == 6);
        REQUIRE(elseBlock->end() == 9);
      }

      THEN("The blocks have the correct parent relationships") {
        REQUIRE(condBlock->parents().empty());

        REQUIRE(thenBlock->parents().size() == 1);
        REQUIRE(thenBlock->parents().at(0).lock() == condBlock);

        REQUIRE(elseBlock->parents().size() == 1);
        REQUIRE(elseBlock->parents().at(0).lock() == condBlock);
      }

      THEN("The blocks have the correct child relationships") {
        REQUIRE(condBlock->children().size() == 2);
        REQUIRE(condBlock->children().at(0).lock() == thenBlock);
        REQUIRE(condBlock->children().at(1).lock() == elseBlock);

        REQUIRE(thenBlock->children().empty());

        REQUIRE(elseBlock->children().empty());
      }
    }
  }
}

SCENARIO(
    "CFGExtractor can handle procedures containing one container statement "
    "with non-container statements surrounding it",
    "[sp][sp/design_extractor][sp/design_extractor/cfg_extractor]") {
  GIVEN("A simple program with one procedure") {
    std::string program = R"(
      procedure main {
        a = 1;
        if (a == 1) then {
          a = 1;
          b = 2;
          print x;
          read y;
        } else {
          c = 3;
          d = 4;
          print a;
          print b;
        }
        a = 2;
      })";

    auto programNode = ProgramStringToProgramNode(program);

    WHEN("The CFGExtractor is run on the program") {
      auto extractor = std::make_shared<design_extractor::CFGExtractor>();
      programNode->AcceptVisitor(programNode, extractor, 0);
      auto cfg = extractor->cfg();

      THEN("The CFGExtractor should have extracted a CFG with 5 blocks") {
        REQUIRE(cfg->Size() == 5);
      }

      THEN(
          "The correct blocks are returned with GetBlockAt() for any valid "
          "statement number") {
        // for dupe check
        std::unordered_set<std::shared_ptr<cfg::Block>> blocks;

        auto stmtBlock1 = cfg->GetBlockAt(1);
        REQUIRE(stmtBlock1.has_value());
        blocks.insert(stmtBlock1.value());

        auto condBlock = cfg->GetBlockAt(2);
        REQUIRE(condBlock.has_value());
        blocks.insert(condBlock.value());

        auto thenBlock = cfg->GetBlockAt(3);
        REQUIRE(thenBlock.has_value());
        blocks.insert(thenBlock.value());

        auto elseBlock = cfg->GetBlockAt(7);
        REQUIRE(elseBlock.has_value());
        blocks.insert(elseBlock.value());

        auto stmtBlock2 = cfg->GetBlockAt(11);
        REQUIRE(stmtBlock2.has_value());
        blocks.insert(stmtBlock2.value());

        REQUIRE(blocks.size() == 5);

        // just in case
        blocks.insert(stmtBlock2.value());
        REQUIRE(blocks.size() == 5);

        REQUIRE(cfg->GetBlockAt(4) == thenBlock);
        REQUIRE(cfg->GetBlockAt(5) == thenBlock);
        REQUIRE(cfg->GetBlockAt(6) == thenBlock);

        REQUIRE(cfg->GetBlockAt(8) == elseBlock);
        REQUIRE(cfg->GetBlockAt(9) == elseBlock);
        REQUIRE(cfg->GetBlockAt(10) == elseBlock);
      }

      // at this point we should've validated that the blocks exist
      auto stmtBlock1 = cfg->GetBlockAt(1).value();
      auto condBlock = cfg->GetBlockAt(2).value();
      auto thenBlock = cfg->GetBlockAt(3).value();
      auto elseBlock = cfg->GetBlockAt(7).value();
      auto stmtBlock2 = cfg->GetBlockAt(11).value();

      THEN("The blocks have the correct start and end statement numbers") {
        REQUIRE(stmtBlock1->start() == 1);
        REQUIRE(stmtBlock1->end() == 1);

        REQUIRE(condBlock->start() == 2);
        REQUIRE(condBlock->end() == 2);

        REQUIRE(thenBlock->start() == 3);
        REQUIRE(thenBlock->end() == 6);

        REQUIRE(elseBlock->start() == 7);
        REQUIRE(elseBlock->end() == 10);

        REQUIRE(stmtBlock2->start() == 11);
        REQUIRE(stmtBlock2->end() == 11);
      }

      THEN("The blocks have the correct parent relationships") {
        REQUIRE(stmtBlock1->parents().empty());

        REQUIRE(condBlock->parents().size() == 1);
        REQUIRE(condBlock->parents().at(0).lock() == stmtBlock1);

        REQUIRE(thenBlock->parents().size() == 1);
        REQUIRE(thenBlock->parents().at(0).lock() == condBlock);

        REQUIRE(elseBlock->parents().size() == 1);
        REQUIRE(elseBlock->parents().at(0).lock() == condBlock);

        REQUIRE(stmtBlock2->parents().size() == 2);
        REQUIRE(stmtBlock2->parents().at(0).lock() == thenBlock);
        REQUIRE(stmtBlock2->parents().at(1).lock() == elseBlock);
      }

      THEN("The blocks have the correct child relationships") {
        REQUIRE(stmtBlock1->children().size() == 1);
        REQUIRE(stmtBlock1->children().at(0).lock() == condBlock);

        REQUIRE(condBlock->children().size() == 2);
        REQUIRE(condBlock->children().at(0).lock() == thenBlock);
        REQUIRE(condBlock->children().at(1).lock() == elseBlock);

        REQUIRE(thenBlock->children().size() == 1);
        REQUIRE(thenBlock->children().at(0).lock() == stmtBlock2);

        REQUIRE(elseBlock->children().size() == 1);
        REQUIRE(elseBlock->children().at(0).lock() == stmtBlock2);

        REQUIRE(stmtBlock2->children().empty());
      }
    }
  }
}

SCENARIO(
    "CFGExtractor can handle procedures containing nested container "
    "statements",
    "[sp][sp/design_extractor][sp/design_extractor/cfg_extractor]") {
  GIVEN("A program with one procedure") {
    std::string program = R"(
        procedure main {
          if (a == 1) then {
            a = 1;
            b = 2;
            if (b == 2) then {
              c = 3;
              d = 4;
            } else {
              e = 5;
              print d;
            }
          } else {
            g = 7;
            print g;
          }
          a = 2;
        })";

    auto programNode = ProgramStringToProgramNode(program);

    WHEN("The CFGExtractor is run on the program") {
      auto extractor = std::make_shared<design_extractor::CFGExtractor>();
      programNode->AcceptVisitor(programNode, extractor, 0);
      auto cfg = extractor->cfg();

      THEN("The CFGExtractor should have extracted a CFG with 7 blocks") {
        REQUIRE(cfg->Size() == 7);
      }

      THEN(
          "The correct blocks are returned with GetBlockAt() for any valid "
          "statement number") {
        // for dupe check
        std::unordered_set<std::shared_ptr<cfg::Block>> blocks;

        auto condBlock1 = cfg->GetBlockAt(1);
        REQUIRE(condBlock1.has_value());
        blocks.insert(condBlock1.value());

        auto thenBlock1 = cfg->GetBlockAt(2);
        REQUIRE(thenBlock1.has_value());
        blocks.insert(thenBlock1.value());

        auto condBlock2 = cfg->GetBlockAt(4);
        REQUIRE(condBlock2.has_value());
        blocks.insert(condBlock2.value());

        auto thenBlock2 = cfg->GetBlockAt(5);
        REQUIRE(thenBlock2.has_value());
        blocks.insert(thenBlock2.value());

        auto elseBlock2 = cfg->GetBlockAt(7);
        REQUIRE(elseBlock2.has_value());
        blocks.insert(elseBlock2.value());

        auto elseBlock1 = cfg->GetBlockAt(9);
        REQUIRE(elseBlock1.has_value());
        blocks.insert(elseBlock1.value());

        auto stmtBlock1 = cfg->GetBlockAt(11);
        REQUIRE(stmtBlock1.has_value());
        blocks.insert(stmtBlock1.value());

        REQUIRE(blocks.size() == 7);

        // just in case
        blocks.insert(stmtBlock1.value());
        REQUIRE(blocks.size() == 7);

        REQUIRE(cfg->GetBlockAt(3) == thenBlock1);
        REQUIRE(cfg->GetBlockAt(6) == thenBlock2);
        REQUIRE(cfg->GetBlockAt(8) == elseBlock2);
        REQUIRE(cfg->GetBlockAt(10) == elseBlock1);
      }

      // at this point we should've validated that the blocks exist
      auto condBlock1 = cfg->GetBlockAt(1).value();
      auto thenBlock1 = cfg->GetBlockAt(2).value();
      auto condBlock2 = cfg->GetBlockAt(4).value();
      auto thenBlock2 = cfg->GetBlockAt(5).value();
      auto elseBlock2 = cfg->GetBlockAt(7).value();
      auto elseBlock1 = cfg->GetBlockAt(9).value();
      auto stmtBlock1 = cfg->GetBlockAt(11).value();

      THEN(
          "The blocks should have the correct start and end statement "
          "numbers") {
        REQUIRE(condBlock1->start() == 1);
        REQUIRE(condBlock1->end() == 1);

        REQUIRE(thenBlock1->start() == 2);
        REQUIRE(thenBlock1->end() == 3);

        REQUIRE(condBlock2->start() == 4);
        REQUIRE(condBlock2->end() == 4);

        REQUIRE(thenBlock2->start() == 5);
        REQUIRE(thenBlock2->end() == 6);

        REQUIRE(elseBlock2->start() == 7);
        REQUIRE(elseBlock2->end() == 8);

        REQUIRE(elseBlock1->start() == 9);
        REQUIRE(elseBlock1->end() == 10);

        REQUIRE(stmtBlock1->start() == 11);
        REQUIRE(stmtBlock1->end() == 11);
      }

      THEN("The blocks should have the correct parent relationships") {
        REQUIRE(condBlock1->parents().empty());

        REQUIRE(thenBlock1->parents().size() == 1);
        REQUIRE(thenBlock1->parents().at(0).lock() == condBlock1);

        REQUIRE(condBlock2->parents().size() == 1);
        REQUIRE(condBlock2->parents().at(0).lock() == thenBlock1);

        REQUIRE(thenBlock2->parents().size() == 1);
        REQUIRE(thenBlock2->parents().at(0).lock() == condBlock2);

        REQUIRE(elseBlock2->parents().size() == 1);
        REQUIRE(elseBlock2->parents().at(0).lock() == condBlock2);

        REQUIRE(elseBlock1->parents().size() == 1);
        REQUIRE(elseBlock1->parents().at(0).lock() == condBlock1);

        REQUIRE(stmtBlock1->parents().size() == 3);
        REQUIRE(stmtBlock1->parents().at(0).lock() == thenBlock2);
        REQUIRE(stmtBlock1->parents().at(1).lock() == elseBlock2);
        REQUIRE(stmtBlock1->parents().at(2).lock() == elseBlock1);
      }

      THEN("The blocks should have the correct children relationships") {
        REQUIRE(condBlock1->children().size() == 2);
        REQUIRE(condBlock1->children().at(0).lock() == thenBlock1);
        REQUIRE(condBlock1->children().at(1).lock() == elseBlock1);

        REQUIRE(thenBlock1->children().size() == 1);
        REQUIRE(thenBlock1->children().at(0).lock() == condBlock2);

        REQUIRE(condBlock2->children().size() == 2);
        REQUIRE(condBlock2->children().at(0).lock() == thenBlock2);
        REQUIRE(condBlock2->children().at(1).lock() == elseBlock2);

        REQUIRE(thenBlock2->children().size() == 1);
        REQUIRE(thenBlock2->children().at(0).lock() == stmtBlock1);

        REQUIRE(elseBlock2->children().size() == 1);
        REQUIRE(elseBlock2->children().at(0).lock() == stmtBlock1);

        REQUIRE(elseBlock1->children().size() == 1);
        REQUIRE(elseBlock1->children().at(0).lock() == stmtBlock1);

        REQUIRE(stmtBlock1->children().empty());
      }
    }
  }
}
}  // namespace test_design_extractor
