#include "QueryHelper.h"
#include "catch.hpp"

using namespace qps;

TEST_CASE("Queries can be built from parsing and tokenising from inputs") {
  SECTION("Can be built with single select clause only") {

    REQUIRE_NOTHROW(QueryHelper::buildQuery("stmt s; Select s"));
  }

  SECTION("Can be built with single such that clause") {

    REQUIRE_NOTHROW(QueryHelper::buildQuery("stmt s; Select s such that Modifies(s, _)"));
  }

  SECTION("Can be built with single assign pattern clause") {

    REQUIRE_NOTHROW(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (s, _)"));
  }

  SECTION("Can be built with one such that and assign pattern clause") {

    REQUIRE_NOTHROW(QueryHelper::buildQuery("stmt s; assign a; Select s such that Modifies(s, _) pattern a (s, _)"));
    REQUIRE_NOTHROW(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (s, _) such that Modifies(s, _)"));
  }
}
