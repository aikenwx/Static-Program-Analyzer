#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "util/interval_tree.h"
#include "util/small_int_key_interval_tree.h"

// SCENARIO analogue for TEMPLATE_TEST_CASE; basically the same as SCENARIO
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TEMPLATE_SCENARIO(...) TEMPLATE_TEST_CASE("Scenario: " __VA_ARGS__)

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

namespace test_util {
TEMPLATE_SCENARIO("IntervalTree can be constructed",
                  "[util][util/interval_tree]", (util::IntervalTree<int, int>),
                  (util::SmallIntKeyIntervalTree<int, int>)) {
  GIVEN("An IntervalTree") {
    TestType tree;
    WHEN("The tree is constructed") {
      THEN("The tree is empty") { REQUIRE(tree.Search(0) == std::nullopt); }
    }
  }
}

TEMPLATE_SCENARIO("IntervalTree can insert intervals",
                  "[util][util/interval_tree]", (util::IntervalTree<int, int>),
                  (util::SmallIntKeyIntervalTree<int, int>)) {
  GIVEN("An IntervalTree with no intervals") {
    TestType tree;
    WHEN("An interval is inserted") {
      tree.Insert({0, 1}, 0);
      THEN("The interval can be found") {
        REQUIRE(tree.Search(0) == 0);
        REQUIRE(tree.Search(1) == 0);
      }
    }

    WHEN("Multiple intervals are inserted") {
      tree.Insert({0, 1}, 0);
      tree.Insert({2, 3}, 1);
      tree.Insert({4, 5}, 2);
      THEN("The intervals can be found") {
        REQUIRE(tree.Search(0) == 0);
        REQUIRE(tree.Search(1) == 0);
        REQUIRE(tree.Search(2) == 1);
        REQUIRE(tree.Search(3) == 1);
        REQUIRE(tree.Search(4) == 2);
        REQUIRE(tree.Search(5) == 2);
      }
    }

    WHEN("Overlapping intervals are inserted") {
      tree.Insert({1, 4}, 0);
      THEN("Insert() with fully-enclosed interval returns false") {
        REQUIRE(tree.Insert({2, 3}, 1) == false);
      }
      THEN("Insert() with partially-enclosed interval returns false") {
        REQUIRE(tree.Insert({0, 3}, 1) == false);
        REQUIRE(tree.Insert({2, 5}, 1) == false);
      }
      THEN("Insert() with fully-enclosing interval returns false") {
        REQUIRE(tree.Insert({0, 5}, 2) == false);
      }
    }
  }

  GIVEN("An IntervalTree with an interval") {
    TestType tree;
    tree.Insert({0, 1}, 0);
    WHEN("An interval is inserted") {
      tree.Insert({2, 3}, 1);
      THEN("The intervals can be found") {
        REQUIRE(tree.Search(0) == 0);
        REQUIRE(tree.Search(1) == 0);
        REQUIRE(tree.Search(2) == 1);
        REQUIRE(tree.Search(3) == 1);
      }
    }

    WHEN("A duplicate interval is inserted") {
      THEN("Insert() returns false, and the existing interval is not changed") {
        REQUIRE(tree.Insert({0, 1}, 3) == false);
        REQUIRE(tree.Search(0) == 0);
      }
    }
  }
}

TEMPLATE_SCENARIO("IntervalTree can delete intervals",
                  "[util][util/interval_tree]", (util::IntervalTree<int, int>),
                  (util::SmallIntKeyIntervalTree<int, int>)) {
  GIVEN("An IntervalTree with an interval") {
    TestType tree;
    tree.Insert({0, 1}, 0);
    WHEN("The interval is deleted") {
      tree.Delete({0, 1});
      THEN("The interval cannot be found") {
        REQUIRE(tree.Search(0) == std::nullopt);
        REQUIRE(tree.Search(1) == std::nullopt);
      }
    }

    WHEN("An nonexistent interval is deleted") {
      THEN("No intervals are deleted") {
        REQUIRE(tree.Delete({2, 3}) == false);
        REQUIRE(tree.Search(0) == 0);
        REQUIRE(tree.Search(1) == 0);
      }
    }

    WHEN("An invalid interval is deleted") {
      THEN("No intervals are deleted") {
        REQUIRE(tree.Delete({1, 0}) == false);
        REQUIRE(tree.Search(0) == 0);
        REQUIRE(tree.Search(1) == 0);
      }
    }
  }

  GIVEN("An IntervalTree with multiple intervals") {
    TestType tree;
    tree.Insert({0, 1}, 0);
    tree.Insert({2, 3}, 1);
    tree.Insert({4, 5}, 2);
    WHEN("An interval is deleted") {
      tree.Delete({2, 3});
      THEN("The remaining intervals, but not the deleted intervals, can be "
           "found") {
        REQUIRE(tree.Search(0) == 0);
        REQUIRE(tree.Search(1) == 0);
        REQUIRE(tree.Search(2) == std::nullopt);
        REQUIRE(tree.Search(3) == std::nullopt);
        REQUIRE(tree.Search(4) == 2);
        REQUIRE(tree.Search(5) == 2);
      }
    }

    WHEN("Multiple intervals are deleted") {
      tree.Delete({0, 1});
      tree.Delete({4, 5});
      THEN("The remaining intervals, but not the deleted intervals, can be "
           "found") {
        REQUIRE(tree.Search(0) == std::nullopt);
        REQUIRE(tree.Search(1) == std::nullopt);
        REQUIRE(tree.Search(2) == 1);
        REQUIRE(tree.Search(3) == 1);
        REQUIRE(tree.Search(4) == std::nullopt);
        REQUIRE(tree.Search(5) == std::nullopt);
      }
    }
  }

  GIVEN("An IntervalTree with no intervals") {
    TestType tree;
    WHEN("A deletion is attempted") {
      THEN("Delete() returns false") { REQUIRE(tree.Delete({0, 1}) == false); }
    }
  }
}

TEMPLATE_SCENARIO("IntervalTree can search for intervals",
                  "[util][util/interval_tree]", (util::IntervalTree<int, int>),
                  (util::SmallIntKeyIntervalTree<int, int>)) {
  GIVEN("An IntervalTree with no intervals") {
    TestType tree;
    WHEN("The tree is searched") {
      THEN("The tree returns std::nullopt") {
        REQUIRE(tree.Search(0) == std::nullopt);
      }
    }
  }

  GIVEN("An IntervalTree with an interval") {
    TestType tree;
    tree.Insert({0, 1}, 0);
    WHEN("The tree is searched for an existing value") {
      THEN("Search() returns the correct value") {
        REQUIRE(tree.Search(0) == 0);
      }
    }

    WHEN("The tree is searched for a value that is not in the interval") {
      THEN("Search() returns std::nullopt") {
        REQUIRE(tree.Search(2) == std::nullopt);
      }
    }
  }

  GIVEN("An IntervalTree with multiple intervals") {
    TestType tree;
    tree.Insert({0, 1}, 0);
    tree.Insert({2, 3}, 1);
    tree.Insert({4, 5}, 2);
    WHEN("The tree is searched") {
      THEN("Search() returns the correct values") {
        REQUIRE(tree.Search(0) == 0);
        REQUIRE(tree.Search(1) == 0);
        REQUIRE(tree.Search(2) == 1);
        REQUIRE(tree.Search(3) == 1);
        REQUIRE(tree.Search(4) == 2);
        REQUIRE(tree.Search(5) == 2);
      }
    }
  }
}
} // namespace test_util

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
