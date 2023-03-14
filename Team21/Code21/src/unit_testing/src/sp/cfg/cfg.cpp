#include "sp/cfg/cfg.h"

#include <catch2/catch_test_macros.hpp>

namespace test_cfg {
SCENARIO("Simple CFG with no blocks", "[sp][sp/cfg][sp/cfg/cfg]") {
  GIVEN("A CFG with no blocks") {
    auto cfg = std::make_shared<cfg::CFG>();

    WHEN("When the size is requested") {
      THEN("The size is 0") { REQUIRE(cfg->Size() == 0); }
    }

    WHEN("When a block is requested") {
      THEN("The block is not found") { REQUIRE(!cfg->GetBlockAt(1).has_value()); }
    }
  }
}

SCENARIO("CFG with blocks", "[sp][sp/cfg][sp/cfg/cfg]") {
  GIVEN("A CFG with a block") {
    auto cfg = std::make_shared<cfg::CFG>();
    auto block = std::make_shared<cfg::Block>(1, 2);
    cfg->InsertBlock(block);

    WHEN("When the size is requested") {
      THEN("The size is 1") { REQUIRE(cfg->Size() == 1); }
    }

    WHEN("When a block is requested") {
      THEN("The block is found") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());
        REQUIRE(block_ptr.value() == block);
      }
    }

    WHEN("A non-existent block is requested") {
      THEN("The block is not found") {
        REQUIRE(!cfg->GetBlockAt(3).has_value());
      }
    }
  }

  GIVEN("A CFG with multiple blocks") {
    auto cfg = std::make_shared<cfg::CFG>();
    auto block1 = std::make_shared<cfg::Block>(1, 2);
    auto block2 = std::make_shared<cfg::Block>(3, 4);
    auto block3 = std::make_shared<cfg::Block>(5, 6);
    cfg->InsertBlock(block1);
    cfg->InsertBlock(block2);
    cfg->InsertBlock(block3);

    WHEN("When the size is requested") {
      THEN("The size is 3") { REQUIRE(cfg->Size() == 3); }
    }

    WHEN("When any existing block is requested") {
      THEN("The blocks are found") {
        auto block_ptr = cfg->GetBlockAt(1);
        REQUIRE(block_ptr.has_value());
        REQUIRE(block_ptr.value() == block1);
        block_ptr = cfg->GetBlockAt(2);
        REQUIRE(block_ptr.has_value());
        REQUIRE(block_ptr.value() == block1);
        block_ptr = cfg->GetBlockAt(3);
        REQUIRE(block_ptr.has_value());
        REQUIRE(block_ptr.value() == block2);
        block_ptr = cfg->GetBlockAt(4);
        REQUIRE(block_ptr.has_value());
        REQUIRE(block_ptr.value() == block2);
        block_ptr = cfg->GetBlockAt(5);
        REQUIRE(block_ptr.has_value());
        REQUIRE(block_ptr.value() == block3);
        block_ptr = cfg->GetBlockAt(6);
        REQUIRE(block_ptr.has_value());
        REQUIRE(block_ptr.value() == block3);
      }
    }

    WHEN("A non-existent block is requested") {
      THEN("The block is not found") {
        REQUIRE(!cfg->GetBlockAt(7).has_value());
      }
    }
  }
}
}  // namespace test_cfg
