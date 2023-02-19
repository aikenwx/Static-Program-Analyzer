#include <utility>

#include "catch.hpp"

#include "PopulatePKBHelper.cpp"
#include "QPSUtilities.h"

//Sample program
//procedure computeCentroid {
//    01      count = 1+2;    -> "1""2"+
//    02      count = count+1 -> "count""1"+
//    03      cenX = 2*4+5*6  -> "2""4"*"5""6"*+
//    04      cenY = 2*count+1+2*cenX -> "2""count"*"1""2""cenX"*++

TEST_CASE("QPS can work with different combinations of assign pattern") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY"};
  data[qps::DesignEntity::CONSTANT] = {"1", "2", "4", "5", "6"};
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "3", "4"};

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddStatementModifies({{1, "count"}, {2, "count"}, {3, "cenX"}, {4, "cenY"}});

  pkb_helper.AddPostFixExpressions({{1, R"("1""2"+)"},
                                    {2, R"("count""1"+)"},
                                    {3, R"("2""4"*"5""6"*+)"},
                                    {4, R"("2""count"*"1""2""cenX"*++)"}});

  SECTION("Can do partial match for a single constant") {
    std::unordered_set<std::string> expected{"1", "2", "4"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"1\"_)", *pkb_querier) == expected);
    expected = {"1", "3", "4"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"2\"_)", *pkb_querier) == expected);

  }

  SECTION("Can do partial match for a single variable name") {
    std::unordered_set<std::string> expected{"2", "4"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"count\"_)", *pkb_querier) == expected);
  }

  SECTION("Returns nth for non existing match on expression") {
    std::unordered_set<std::string> empty{};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"count1\"_)", *pkb_querier) == empty);
  }

  SECTION("Can handle two wildcard") {
    std::unordered_set<std::string> expected{"1", "2", "3", "4"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _)", *pkb_querier) == expected);
  }

  SECTION("Can handle lhs synonm, right wildcard") {
    std::unordered_set<std::string> expected{"count", "cenX", "cenY"};
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select v pattern a(v, _)", *pkb_querier) == expected);
    expected = {"1", "2", "3", "4"};
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select a pattern a(v, _)", *pkb_querier) == expected);
  }

  SECTION("Can handle lhs synonym, with right expression") {
    std::unordered_set<std::string> expected{"count", "cenY"};
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select v pattern a(v,_\"1\"_)", *pkb_querier) == expected);
    expected = {"1", "2", "4"};
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select a pattern a(v, _\"1\"_)", *pkb_querier) == expected);
  }

  SECTION("Can handle lhs literal, with right expression") {
    std::unordered_set<std::string> expected = {"4"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(\"cenY\",_\"1\"_)", *pkb_querier) == expected);
    std::unordered_set<std::string> empty = {};
    REQUIRE(
        qps_test::RunQuery("assign a; variable v; Select a pattern a(\"cenX\", _\"1\"_)", *pkb_querier) == empty);
  }
}
