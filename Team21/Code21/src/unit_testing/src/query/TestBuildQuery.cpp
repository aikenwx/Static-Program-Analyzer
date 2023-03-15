#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include "QueryHelper.h"

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

TEST_CASE("Build queries with invalid expression spec. Check if throw syntax errors") {
  using Catch::Matchers::Message;
  SECTION("Check no leading zero for number") {
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"0121212 + 456\"_)"),
      "Syntactic error. Expression spec contains leading zero");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"012\"_)"),
      "Syntactic error. Expression spec contains leading zero");
  }

  SECTION("Check no integer can contain a letter in it") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"190293333a + 123\"_)"),
      "Syntactic error. Integer has a letter in it");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"19r89\"_)"),
      "Syntactic error. Integer has a letter in it");
  }

  SECTION("Check no ) as start") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\") + asdasd\"_)"),
      "Syntactic error. ) cannot be first char");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\")\"_)"),
      "Syntactic error. ) cannot be first char");
  }

  SECTION("Check no operator as start") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"*123\"_)"),
      "Syntactic error. Operator cannot be first char");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"%(a9876)\"_)"),
      "Syntactic error. Operator cannot be first char");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"*agt3\"_)"),
      "Syntactic error. Operator cannot be first char");
  }

  SECTION("Check no weird symbols allowed") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"&\"_)"),
      "Syntactic error. Expression spec contains unallowed characters inside or ends with wrong character");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"^\"_)"),
      "Syntactic error. Expression spec contains unallowed characters inside or ends with wrong character");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"&&^^\"_)"),
      "Syntactic error. Expression spec contains unallowed characters inside or ends with wrong character");
  }

  SECTION("Check cannot terminate with operator or (") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"1+\"_)"),
      "Syntactic error. Expression spec contains unallowed characters inside or ends with wrong character");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"1+(\"_)"),
      "Syntactic error. Expression spec contains unallowed characters inside or ends with wrong character");
  }

  SECTION("Check ( has to be followed by alphanumeric or (") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"(*)\"_)"),
      "Syntactic error. ( is followed by neither an alphanumeric char or (");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"()\"_)"),
      "Syntactic error. ( is followed by neither an alphanumeric char or (");
  }

  SECTION("Check ( has to preceded by operator or (") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"abc(a + 2)\"_)"),
      "Syntactic error. ( is preceded by neither an operator or (");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"(1245(a + 2)\"_)"),
      "Syntactic error. ( is preceded by neither an operator or (");
  }

  SECTION("Check ) has to be followed by operator or )") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"(123)123\"_)"),
      "Syntactic error. ) is followed by neither an operator or )");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"(123)(adadad)\"_)"),
      "Syntactic error. ) is followed by neither an operator or )");
  }

  SECTION("Check ) has to be preceded by alphanumeric or )") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"(1*)\"_)"),
      "Syntactic error. ) is preceded by neither an alphanumeric char or )");
  }

  SECTION("Check cannot have closing bracket without a preceding opening bracket to match it") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"a)\"_)"),
      "Syntactic error. ) is not supposed to be allowed without a opening bracket");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"(1 + 2) + axccx34345)\"_)"),
      "Syntactic error. ) is not supposed to be allowed without a opening bracket");
  }

  SECTION("operator cannot be followed by an operator") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"1+*\"_)"),
      "Syntactic error. operator cannot be followed by operator");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"1+2*a%-\"_)"),
      "Syntactic error. operator cannot be followed by operator");
  }

  SECTION("Check that number of opening and closing cannot not be equal") {

    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"(((1+abcdef)\"_)"),
      "Syntactic error. Closing brackets are insufficient");
    REQUIRE_THROWS_WITH(QueryHelper::buildQuery("stmt s; assign a; Select s pattern a (_, _\"(((1+abcdef*(Ad129090))\"_)"),
      "Syntactic error. Closing brackets are insufficient");
  }
}
