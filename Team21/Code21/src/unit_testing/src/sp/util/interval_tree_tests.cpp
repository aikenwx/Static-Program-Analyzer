#include <util/interval_tree.h>

#include <catch.hpp>

namespace test_util {
SCENARIO("IntervalTree can be constructed", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
    WHEN("The tree is constructed") {
      THEN("The tree is empty") { REQUIRE(tree.Search(0) == std::nullopt); }
    }
  }
}

SCENARIO("IntervalTree can insert intervals", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
    WHEN("An interval is inserted") {
      tree.Insert({0, 1}, 0);
      THEN("The interval can be found") {
        REQUIRE(tree.Search(0) == 0);
        REQUIRE(tree.Search(1) == 0);
      }
    }
  }
}

SCENARIO("IntervalTree can delete intervals", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree with an interval") {
    util::IntervalTree<int, int> tree;
    tree.Insert({0, 1}, 0);
    WHEN("The interval is deleted") {
      tree.Delete({0, 1});
      THEN("The interval cannot be found") {
        REQUIRE(tree.Search(0) == std::nullopt);
        REQUIRE(tree.Search(1) == std::nullopt);
      }
    }
  }
}

SCENARIO("IntervalTree can insert multiple intervals", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
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
  }
}

SCENARIO("IntervalTree can delete multiple intervals", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree with multiple intervals") {
    util::IntervalTree<int, int> tree;
    tree.Insert({0, 1}, 0);
    tree.Insert({2, 3}, 1);
    tree.Insert({4, 5}, 2);
    WHEN("The intervals are deleted") {
      tree.Delete({0, 1});
      tree.Delete({2, 3});
      tree.Delete({4, 5});
      THEN("The intervals cannot be found") {
        REQUIRE(tree.Search(0) == std::nullopt);
        REQUIRE(tree.Search(1) == std::nullopt);
        REQUIRE(tree.Search(2) == std::nullopt);
        REQUIRE(tree.Search(3) == std::nullopt);
        REQUIRE(tree.Search(4) == std::nullopt);
        REQUIRE(tree.Search(5) == std::nullopt);
      }
    }
  }
}

SCENARIO("IntervalTree.Search() returns nullopt when the value is not found", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
    tree.Insert({1, 5}, 2);
    WHEN("The tree is searched") {
      THEN("The tree returns nullopt") { REQUIRE(tree.Search(0) == std::nullopt); }
    }
  }
}

SCENARIO("IntervalTree.Search() returns nullopt when the tree is empty", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
    WHEN("The tree is searched") {
      THEN("The tree returns nullopt") { REQUIRE(tree.Search(0) == std::nullopt); }
    }
  }
}

SCENARIO("IntervalTree.Insert() returns false with overlapping intervals", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
    WHEN("An overlapping interval is inserted") {
      tree.Insert({0, 5}, 0);
      THEN("The interval is not inserted, and cannot be found") {
        REQUIRE(tree.Insert({2, 4}, 1) == false);
        REQUIRE(tree.Search(2) != 1);
      }
    }
  }
}

SCENARIO("IntervalTree.Insert() returns false with invalid intervals", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
    WHEN("An invalid interval is inserted") {
      THEN("The interval is not inserted") {
        REQUIRE(tree.Insert({1, 0}, 0) == false);
      }
    }
  }
}

SCENARIO("IntervalTree.Insert() returns false with duplicate intervals", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
    WHEN("A duplicate interval is inserted") {
      tree.Insert({0, 1}, 0);
      THEN("The interval is not inserted") {
        REQUIRE(tree.Insert({0, 1}, 0) == false);
      }
    }
  }
}

SCENARIO("IntervalTree.Delete() returns false when trying to delete a non-existent interval", "[util][util/interval_tree]") {
  GIVEN("An IntervalTree") {
    util::IntervalTree<int, int> tree;
    WHEN("A non-existent interval is deleted") {
      THEN("The interval is not deleted") {
        REQUIRE(tree.Delete({0, 1}) == false);
      }
    }
  }
}
}  // namespace util
