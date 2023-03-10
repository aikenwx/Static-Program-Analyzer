#include <catch2/catch_test_macros.hpp>
#include <utility>

#include "PopulatePKBHelper.cpp"
#include "QPSUtilities.h"

//Sample program
//procedure computeCentroid {
//    01      count = 1+2;    -> "1""2"+
//    02      count = count+1 -> "count""1"+
//    03      cenX = 2*4+5*6  -> "2""4"*"5""6"*+
//    04      cenY = 2*count+1+2*cenX -> "2""count"*"1"+"2""cenX"*+
//    05      cenZ = 1%2+(4+2) -> "1""2"%"4""2"++

TEST_CASE("QPS can work with different combinations of assign pattern") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY", "cenZ"};
  data[qps::DesignEntity::CONSTANT] = {"1", "2", "4", "5", "6"};
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "3", "4", "5"};

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddStatementModifies({ {1, "count"}, {2, "count"}, {3, "cenX"}, {4, "cenY"}, {5, "cenZ"}});

  pkb_helper.AddPostFixExpressions({{1, R"("1""2"+)"},
                                    {2, R"("count""1"+)"},
                                    {3, R"("2""4"*"5""6"*+)"},
                                    {4, R"("2""count"*"1"+"2""cenX"*+)"},
                                    {5, R"("1""2"%"4""2"++)"}});

  SECTION("Can do partial match for a single constant") {
    std::unordered_set<std::string> expected{"1", "2", "4", "5"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"1\"_)", *pkb_querier) == expected);
    expected = {"1", "3", "4", "5"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"2\"_)", *pkb_querier) == expected);

  }

  SECTION("Can do partial match for expression-spec with lhs wildcard") {
    std::unordered_set<std::string> expected{ "3" };
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"5*6\"_)", *pkb_querier) == expected);
    expected = { "4" };
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"2*count+ 1\"_)", *pkb_querier) == expected);

  }

  SECTION("Can do partial match for expression-spec and fail if not subexpression. Fail also if it is substring of infix and not subexpression") {
    std::unordered_set<std::string> expected{ };
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"4+5\"_)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"1+2*cenX\"_)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\"1%2+4\"_)", *pkb_querier) == expected);
  }

  SECTION("Check if it allows Whitespaces (including multiple spaces, tabs, or no spaces)") {
      std::unordered_set<std::string> expected{ "1", "2", "4", "5"};
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_,     _     \"1 \"_)", *pkb_querier) == expected);
      expected = { "1", "3", "4", "5"};
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\" 2\"_)", *pkb_querier) == expected);
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _\" 2         \"_)", *pkb_querier) == expected);
      REQUIRE(qps_test::RunQuery("assign   a   ;   Select    a       pattern   a   ( _   , _\" 2         \"_)", *pkb_querier) == expected);

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
    std::unordered_set<std::string> expected{"1", "2", "3", "4", "5"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, _)", *pkb_querier) == expected);
  }

  SECTION("Can handle lhs synonm, right wildcard") {
    std::unordered_set<std::string> expected{"count", "cenX", "cenY", "cenZ"};
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select v pattern a(v, _)", *pkb_querier) == expected);
    expected = {"1", "2", "3", "4", "5"};
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select a pattern a(v, _)", *pkb_querier) == expected);
  }

  SECTION("Can handle lhs synonym, with right expression constant") {
    std::unordered_set<std::string> expected{"count", "cenY", "cenZ"};
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select v pattern a(v,_\"1\"_)", *pkb_querier) == expected);
    expected = {"1", "2", "4", "5"};
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select a pattern a(v, _\"1\"_)", *pkb_querier) == expected);
  }

  SECTION("Can do partial match for expression-spec with lhs synonym, with right expression") {
    std::unordered_set<std::string> expected{ "3" };
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select a pattern a(v, _\"5*6\"_)", *pkb_querier) == expected);
    expected = { "4" };
    REQUIRE(qps_test::RunQuery("assign a; variable v; Select a pattern a(v, _\"2*count+ 1\"_)", *pkb_querier) == expected);

  }

  SECTION("Can handle lhs literal, with right expression constant") {
    std::unordered_set<std::string> expected = {"4"};
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(\"cenY\",_\"1\"_)", *pkb_querier) == expected);
    std::unordered_set<std::string> empty = {};
    REQUIRE(
        qps_test::RunQuery("assign a; variable v; Select a pattern a(\"cenX\", _\"1\"_)", *pkb_querier) == empty);
  }

  SECTION("Can do partial match for expression-spec with lhs literal, with right expression") {
    std::unordered_set<std::string> expected{ "3" };
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(\"cenX\", _\"5*6\"_)", *pkb_querier) == expected);
    expected = { "4" };
    REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(\"cenY\", _\"2*count+ 1\"_)", *pkb_querier) == expected);

  }

  SECTION("Full expressions") {
    SECTION("Check if it allows Whitespaces (including multiple spaces, tabs, or no spaces) for full expression") {
      std::unordered_set<std::string> expected{ "1" };
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_,          \"1   +     2\")", *pkb_querier) == expected);
      expected = { "2" };
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, \" count +    1   \")", *pkb_querier) == expected);
      expected = { "3" };
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, \" 2 *   4   + 5 *      6         \")", *pkb_querier) == expected);
      expected = { "4" };
      REQUIRE(qps_test::RunQuery("assign   a   ;   Select    a   pattern   a   ( _   , \" 2*c o u n t+1+2*cen X        \")", *pkb_querier) == expected);
      expected = { "5" };
      REQUIRE(qps_test::RunQuery("assign   a   ;   Select    a       pattern   a   ( _   , \"1%2+(   4 + 2   )        \")", *pkb_querier) == expected);
    }

    SECTION("Returns nth for non existing match on full expression") {
      std::unordered_set<std::string> empty{};
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, \"count1\")", *pkb_querier) == empty);
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, \"2*count+1\")", *pkb_querier) == empty);
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, \"5*6\")", *pkb_querier) == empty);
    }

    SECTION("Can do full match for expression-spec with lhs wildcard") {
      std::unordered_set<std::string> expected{ "3" };
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, \"2*4+5*6\")", *pkb_querier) == expected);
      expected = { "4" };
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(_, \"2*count+1+2*cenX\")", *pkb_querier) == expected);

    }

    SECTION("Can do full match for expression-spec with lhs synonym, with right expression") {
      std::unordered_set<std::string> expected{ "3" };
      REQUIRE(qps_test::RunQuery("assign a; variable v; Select a pattern a(v, \"2*4+5*6\")", *pkb_querier) == expected);
      expected = { "4" };
      REQUIRE(qps_test::RunQuery("assign a; variable v; Select a pattern a(v, \"2*count+1+2*cenX\")", *pkb_querier) == expected);

    }

    SECTION("Can do full match for expression-spec with lhs literal, with right expression") {
      std::unordered_set<std::string> expected{ "3" };
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(\"cenX\", \"2*4+5*6\")", *pkb_querier) == expected);
      expected = { "4" };
      REQUIRE(qps_test::RunQuery("assign a; Select a pattern a(\"cenY\", \"2*count+1+2*cenX\")", *pkb_querier) == expected);

    }
  }

  //To be added haha
  SECTION("catching syntx errors in expression-spec") {

  }
}
