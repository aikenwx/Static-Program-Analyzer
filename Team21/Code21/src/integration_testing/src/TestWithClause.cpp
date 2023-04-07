#include <catch2/catch_test_macros.hpp>
#include <utility>

#include "PopulatePKBHelper.cpp"
#include "query/design_entity.h"
#include "QPSUtilities.h"

//Sample program
//procedure computeCentroid{
//    count = 1 + 2 + 3; 01 -> "1""2"+
//    if (y!=0) then { 02
//      count = count + 1; 03 -> "count""1"+
//    } else {
//      cenX = count + 2; 04 -> "count""2"+
//    }
//    while (y!= 5000) { 05
//      y = 2 * 4 + 5 * 6; 06 -> "2""4"*"5""6"*+
//    }
//    cenY = 2*count+1+2*cenX; 07 -> "2""count"*"1""2""cenX"*++
//}


// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

TEST_CASE("With clause queries work with multiple clauses. Only one of each") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY", "y"};
  data[qps::DesignEntity::CONSTANT] = {"0", "1", "2", "3", "4", "5", "6", "5000"};
  data[qps::DesignEntity::ASSIGN] = {"1", "3", "4", "6", "7"};
  data[qps::DesignEntity::READ] = {};
  data[qps::DesignEntity::IF] = {"2"};
  data[qps::DesignEntity::WHILE] = {"5"};
  data[qps::DesignEntity::PRINT] = {};
  data[qps::DesignEntity::PROCEDURE] = {"computeCentroid"};

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddFollows({{1, 2}, {2, 5}, {5, 7}});

  pkb_helper.AddFollowsStar({{1, 2}, {2, 5}, {5, 7}, {2, 7}, {1, 5}, {1, 7}});

  pkb_helper.AddStatementUses({{2, "y"}, {3, "count"}, {4, "count"}, {2, "count"}, {5, "y"},
                               {7, "count"}, {7, "cenX"}});

  pkb_helper.AddStatementModifies({{1, "count"}, {3, "count"}, {4, "cenX"}, {6, "y"}, {7, "cenY"},
                                   {2, "count"}, {2, "cenX"}, {5, "y"}});

  pkb_helper.AddParent({{5, 6}, {2, 3}, {2, 4}});

  pkb_helper.AddPostFixExpressions({{1, R"("3""1""2"++)"},
                                    {3, R"("count""1"+)"},
                                    {4, R"("count""2"+)"},
                                    {6, R"("2""4"*"5""6"*+)"},
                                    {7, R"("2""count"*"1""2""cenX"*++)"}});

  SECTION("No Common Synonyms") {
    SECTION("1 synonym table") {
      std::unordered_set<std::string> expected{"1", "3", "7"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Modifies(1, \"count\") pattern a(_, _\"1\"_) with \"qwer\" = \"qwer\"",
                                 *pkb_querier) == expected);
      expected = {"1"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Modifies(1, \"count\") pattern a(_, _\"1\"_) with a.stmt# = 1",
                                 *pkb_querier) == expected);
      expected = {"3"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Modifies(1, \"count\") pattern a(_, _\"1\"_) with a.stmt# = 3",
                                 *pkb_querier) == expected);
      expected = {"7"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Modifies(1, \"count\") pattern a(_, _\"1\"_) with a.stmt# = 7",
                                 *pkb_querier) == expected);
      expected = {};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Modifies(1, \"count\") pattern a(_, _\"1\"_) with a.stmt# = 10",
                                 *pkb_querier) == expected);
      expected = {"1", "3", "7"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Modifies(1, \"count\") pattern a(_, _\"1\"_) with 1 = 1",
                                 *pkb_querier) == expected);
      expected = {};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Modifies(1, \"count\") pattern a(_, _\"1\"_) with 1 = 4343",
                                 *pkb_querier) == expected);
    }

    SECTION("2 synonym table") {
      std::unordered_set<std::string> expected{"count", "cenY"};
      REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Uses(7, \"cenX\") pattern a(v, _\"1\"_) with 12 = 12",
        *pkb_querier) == expected);
      expected = {"cenY", "count"};
      REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Uses(7, \"cenX\") pattern a(v, _\"1\"_) with \"cenY\" = \"cenY\"",
        *pkb_querier) == expected);
      expected = {};
      REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Uses(7, \"cenX\") pattern a(v, _\"1\"_) with 1 = 12",
        *pkb_querier) == expected);
      expected = {};
      REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Uses(7, \"cenX\") pattern a(v, _\"1\"_) with \"q\" = \"qw\"",
        *pkb_querier) == expected);
      expected = {"cenY"};
      REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Uses(7, \"cenX\") pattern a(v, _\"1\"_) with v.varName = \"cenY\"",
        *pkb_querier) == expected);
      expected = {"count"};
      REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Uses(7, \"cenX\") pattern a(v, _\"1\"_) with a.stmt# = 1",
        *pkb_querier) == expected);
      expected = {};
      REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Uses(7, \"cenX\") pattern a(v, _\"1\"_) with a.stmt# = 4",
        *pkb_querier) == expected);
    }

    SECTION("3 synonym table") {
      std::unordered_set<std::string> expected{"count", "cenY", "cenX", "y"};
      REQUIRE(qps_test::RunQuery("assign a, a1; variable v; Select v such that Modifies(a1, v) pattern a(_, _\"1\"_)\
                                  with a.stmt# = 1",
                                 *pkb_querier) == expected);
    }

    SECTION("3 synonym table") {
      std::unordered_set<std::string> expected{"count", "cenY"};
      REQUIRE(qps_test::RunQuery(
          "assign a, a1; variable v; Select v such that Modifies(a1, \"count\") pattern a(v, _\"1\"_)\
          with a1.stmt# = 1",
          *pkb_querier) == expected);
    }

    SECTION("4 synonym table") {
      std::unordered_set<std::string> expected{"count", "cenY"};
      REQUIRE(qps_test::RunQuery(
          "assign a, a1; variable v, v1; Select v1 such that Modifies(a1, v) pattern a(v1, _\"1\"_)\
          with v.varName = v1.varName",
          *pkb_querier) == expected);
    }
  }

  SECTION("1 common synonym") {
    SECTION("Assign syn common") {
      std::unordered_set<std::string> expected{"6"};
      REQUIRE(qps_test::RunQuery(
          "assign a; while w; Select a such that Parent(w, a) pattern a(_, _\"2\"_)\
          with w.stmt# = 5",
          *pkb_querier) == expected);
    }

    SECTION("2 syn table with variable syn common") {
      std::unordered_set<std::string> expected{"count"};
      REQUIRE(qps_test::RunQuery(
          "assign a; variable v; Select v such that Uses(7, v) pattern a(v, _\"3\"_)\
          with v.varName = \"count\"",
          *pkb_querier) == expected);
    }

    SECTION("2 syn table with assign syn common") {
      std::unordered_set<std::string> expected{"1"};
      REQUIRE(qps_test::RunQuery(
          "assign a, a1; Select a such that Follows*(a, a1) pattern a1(_, _\"2\"_)\
          with a.stmt# = 1",
          *pkb_querier) == expected);
    }

    SECTION("3 syn table with variable syn common") {
      std::unordered_set<std::string> expected{"count"};
      REQUIRE(qps_test::RunQuery("variable v; assign a; stmt s; Select v such that Uses(s, v) pattern a(v, _\"1\"_)\
                                 with s.stmt# = 7",
                                 *pkb_querier) == expected);
    }

    SECTION("3 syn table with assign syn common") {
      std::unordered_set<std::string> expected{"cenY"};
      REQUIRE(qps_test::RunQuery(
          "assign a, a1; variable v; Select v such that Follows*(a, a1) pattern a1(v, _\"2\"_) with a.stmt# = 1",
          *pkb_querier) == expected);
    }
  }

  SECTION("2 common synonyms") {
    std::unordered_set<std::string> expected{"count"};
    REQUIRE(qps_test::RunQuery("variable v; assign a; Select v such that Uses(a, v) pattern a(v, _) with v.varName = \"count\"",
                               *pkb_querier) == expected);
  }

  SECTION("Evaluates to true") {
    REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Modifies(1, \"count\") pattern a(\"cenY\", _\"count\"_) with 100 = 100",
        *pkb_querier) == data[qps::DesignEntity::VARIABLE]);
    REQUIRE(qps_test::RunQuery(
        "variable v; assign a; stmt s; Select v such that Modifies(s, \"count\") pattern a(\"cenY\", _\"count\"_)\
         with s.stmt# = s.stmt#",
        *pkb_querier) == data[qps::DesignEntity::VARIABLE]);
  }

  SECTION("Evaluates to false") {
    std::unordered_set<std::string> empty{};
    REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Modifies(1, \"cenX\") pattern a(\"cenY\", _\"y\"_) with 1 = 1",
        *pkb_querier) == empty);
    REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Modifies(1, \"count\") pattern a(\"cenY\", _\"y\"_) with 1 = 2",
        *pkb_querier) == empty);
    REQUIRE(qps_test::RunQuery(
        "variable v; assign a; Select v such that Modifies(1, \"cenX\") pattern a(\"cenY\", _) with v.varName = \"cenX\"",
        *pkb_querier) == empty);
  }

  SECTION("With clause alone") {
    SECTION("Evaluates to have 1 result/results") {
      std::unordered_set<std::string> expected{"count", "cenX", "cenY", "y"};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; Select v with 1 = 1",
          *pkb_querier) == expected);
      expected = {"7"};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; Select a with a.stmt# = 7",
          *pkb_querier) == expected);
      expected = {"cenX"};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; Select v with v.varName = \"cenX\"",
          *pkb_querier) == expected);
    }
  }

  SECTION("With clause with select tuple/Boolean") {
    SECTION("Evaluates with just with and select") {
      std::unordered_set<std::string> expected{"TRUE"};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; Select BOOLEAN with 1 = 1",
          *pkb_querier) == expected);
      expected = {"FALSE"};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; Select BOOLEAN with 1 = 2",
          *pkb_querier) == expected);
      expected = {"7 cenY", "7 y", "7 count", "7 cenX"};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; Select <a, v.varName> with a.stmt# = 7",
          *pkb_querier) == expected);
      expected = {"cenX cenX"};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; Select <v, v.varName> with v.varName = \"cenX\"",
          *pkb_querier) == expected);
      expected = {};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; procedure p; Select <v, v.varName> with p.procName = \"cenX\"",
          *pkb_querier) == expected);
      expected = {};
      REQUIRE(qps_test::RunQuery(
          "variable v; assign a; procedure p; Select <v, v.varName> with v.varName = \"cedsnX\"",
          *pkb_querier) == expected);
    }
  }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
