#include "sp/cfg/block.h"

#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

namespace test_cfg {
SCENARIO("Simple Blocks with no relations to other blocks",
         "[sp][sp/cfg][sp/cfg/block]") {
  GIVEN("A block without any relation to other blocks") {
    auto block = std::make_shared<cfg::Block>(4, 7);

    WHEN("When the block is created") {
      THEN("The start and end statement numbers are correct") {
        REQUIRE(block->start() == 4);
        REQUIRE(block->end() == 7);
      }

      THEN(
          "IsInBlock() returns true for any statement between the start and "
          "end, and not for other blocks") {
        REQUIRE(!block->IsInBlock(1));
        REQUIRE(!block->IsInBlock(3));
        REQUIRE(block->IsInBlock(4));
        REQUIRE(block->IsInBlock(5));
        REQUIRE(block->IsInBlock(6));
        REQUIRE(block->IsInBlock(7));
        REQUIRE(!block->IsInBlock(8));
        REQUIRE(!block->IsInBlock(10));
      }
    }
  }
}

SCENARIO("Blocks with relations to other blocks",
         "[sp][sp/cfg][sp/cfg/block]") {
  GIVEN("A block with a parent and a child") {
    auto parent = std::make_shared<cfg::Block>(4, 7);
    auto child = std::make_shared<cfg::Block>(8, 10);

    WHEN("The parent is added to the child") {
      child->AddParent(parent);

      THEN("References to each other can be retrieved from either of them") {
        REQUIRE(parent->children().size() == 1);
        REQUIRE(parent->children()[0].lock() == child);
        REQUIRE(child->parents().size() == 1);
        REQUIRE(child->parents()[0].lock() == parent);
      }
    }

    WHEN("The child is added to the parent") {
      parent->AddChild(child);

      THEN("References to each other can be retrieved from either of them") {
        REQUIRE(parent->children().size() == 1);
        REQUIRE(parent->children()[0].lock() == child);
        REQUIRE(child->parents().size() == 1);
        REQUIRE(child->parents()[0].lock() == parent);
      }
    }
  }

  GIVEN("A block with many parents") {
    auto parent1 = std::make_shared<cfg::Block>(4, 7);
    auto parent2 = std::make_shared<cfg::Block>(8, 10);
    auto parent3 = std::make_shared<cfg::Block>(11, 13);
    auto child = std::make_shared<cfg::Block>(14, 16);

    WHEN("The parents are added to the child") {
      child->AddParent(parent1);
      child->AddParent(parent2);
      child->AddParent(parent3);

      THEN("References to each other can be retrieved from either of them") {
        REQUIRE(parent1->children().size() == 1);
        REQUIRE(parent1->children()[0].lock() == child);
        REQUIRE(parent2->children().size() == 1);
        REQUIRE(parent2->children()[0].lock() == child);
        REQUIRE(parent3->children().size() == 1);
        REQUIRE(parent3->children()[0].lock() == child);
        REQUIRE(child->parents().size() == 3);
        REQUIRE(child->parents()[0].lock() == parent1);
        REQUIRE(child->parents()[1].lock() == parent2);
        REQUIRE(child->parents()[2].lock() == parent3);
      }
    }

    WHEN("The child is added to the parents") {
      parent1->AddChild(child);
      parent2->AddChild(child);
      parent3->AddChild(child);

      THEN("References to each other can be retrieved from either of them") {
        REQUIRE(parent1->children().size() == 1);
        REQUIRE(parent1->children()[0].lock() == child);
        REQUIRE(parent2->children().size() == 1);
        REQUIRE(parent2->children()[0].lock() == child);
        REQUIRE(parent3->children().size() == 1);
        REQUIRE(parent3->children()[0].lock() == child);
        REQUIRE(child->parents().size() == 3);
        REQUIRE(child->parents()[0].lock() == parent1);
        REQUIRE(child->parents()[1].lock() == parent2);
        REQUIRE(child->parents()[2].lock() == parent3);
      }
    }
  }

  GIVEN("A block with many children") {
    auto parent = std::make_shared<cfg::Block>(4, 7);
    auto child1 = std::make_shared<cfg::Block>(8, 10);
    auto child2 = std::make_shared<cfg::Block>(11, 13);
    auto child3 = std::make_shared<cfg::Block>(14, 16);

    WHEN("The children are added to the parent") {
      parent->AddChild(child1);
      parent->AddChild(child2);
      parent->AddChild(child3);

      THEN("References to each other can be retrieved from either of them") {
        REQUIRE(parent->children().size() == 3);
        REQUIRE(parent->children()[0].lock() == child1);
        REQUIRE(parent->children()[1].lock() == child2);
        REQUIRE(parent->children()[2].lock() == child3);
        REQUIRE(child1->parents().size() == 1);
        REQUIRE(child1->parents()[0].lock() == parent);
        REQUIRE(child2->parents().size() == 1);
        REQUIRE(child2->parents()[0].lock() == parent);
        REQUIRE(child3->parents().size() == 1);
        REQUIRE(child3->parents()[0].lock() == parent);
      }
    }

    WHEN("The parent is added to the children") {
      child1->AddParent(parent);
      child2->AddParent(parent);
      child3->AddParent(parent);

      THEN("References to each other can be retrieved from either of them") {
        REQUIRE(parent->children().size() == 3);
        REQUIRE(parent->children()[0].lock() == child1);
        REQUIRE(parent->children()[1].lock() == child2);
        REQUIRE(parent->children()[2].lock() == child3);
        REQUIRE(child1->parents().size() == 1);
        REQUIRE(child1->parents()[0].lock() == parent);
        REQUIRE(child2->parents().size() == 1);
        REQUIRE(child2->parents()[0].lock() == parent);
        REQUIRE(child3->parents().size() == 1);
        REQUIRE(child3->parents()[0].lock() == parent);
      }
    }
  }
}
}  // namespace test_cfg

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
