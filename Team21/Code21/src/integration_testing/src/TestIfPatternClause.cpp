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
//    06      if (1 == 3) {
//    07        print(1);
//            }
//    08      if (cenZ > cenY) {
//    09        print(count);
//            }
//    10      if (count == cenZ) {
//    11        print(2);
//            }
//    12      if (count > 1) {
//    13        print(3);
//            }

TEST_CASE("QPS can work with different combinations of if pattern") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY", "cenZ"};
  data[qps::DesignEntity::CONSTANT] = {"1", "2", "4", "5", "6"};
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "3", "4", "5"};
  data[qps::DesignEntity::PRINT] = {"7", "9", "11", "13"};
  data[qps::DesignEntity::IF] = {"6", "8", "10", "12"};

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddStatementModifies({{1, "count"}, {2, "count"}, {3, "cenX"}, {4, "cenY"}, {5, "cenZ"}});
  pkb_helper.AddStatementUses({{2, "count"}, {4, "count"}, {4, "cenX"}, {8, "cenY"}, {8, "cenZ"}, {9, "count"},
                               {10, "count"}, {10, "cenZ"},
                               {12, "count"}});

  pkb_helper.AddPostFixExpressions({{1, R"("1""2"+)"},
                                    {2, R"("count""1"+)"},
                                    {3, R"("2""4"*"5""6"*+)"},
                                    {4, R"("2""count"*"1"+"2""cenX"*+)"},
                                    {5, R"("1""2"%"4""2"++)"}});

  pkb_helper.AddIfStatementConditionVariable({{8, R"(cenZ)"},
                                              {8, R"(cenY)"},
                                              {10, R"(count)"},
                                              {10, R"(cenZ)"},
                                              {12, R"(count)"}});

//  SECTION("Can check for if statements with synonym") {
//    std::unordered_set<std::string> expected{ "8", "10", "12" };
//    REQUIRE(qps_test::RunQuery("if i; variable v; Select i pattern i(v, _, _)", *pkb_querier) == expected);
//  }

//  SECTION("Can retrive if statements with underscore") {
//    std::unordered_set<std::string> expected{ "8", "10", "12" };
//    REQUIRE(qps_test::RunQuery("if i; variable v; Select i pattern i(_, _, _)", *pkb_querier) == expected);
//  }

  SECTION("Can retrieve if statements with quoted identifier") {
    std::unordered_set<std::string> expected{"10", "12"};
    REQUIRE(qps_test::RunQuery("if i; variable v; Select i pattern i(\"count\", _, _)", *pkb_querier) == expected);
    expected = {"8"};
    REQUIRE(qps_test::RunQuery("if i; variable v; Select i pattern i(\"cenY\", _, _)", *pkb_querier) == expected);
  }

//  SECTION("Can check for if statements with synonym and return different if synonym") {
//    std::unordered_set<std::string> expected{ "6", "8", "10", "12" };
//    REQUIRE(qps_test::RunQuery("if i, q; variable v; Select q pattern i(v, _, _)", *pkb_querier) == expected);
//  }
//
//  SECTION("Can retrive if statements with underscore and return different if synonym") {
//    std::unordered_set<std::string> expected{ "6", "8", "10", "12" };
//    REQUIRE(qps_test::RunQuery("if i, q; variable v; Select q pattern i(_, _, _)", *pkb_querier) == expected);
//  }
//
//  SECTION("Can retrieve if statements with quoted identifier and return different if synonym") {
//    std::unordered_set<std::string> expected{ "6", "8", "10", "12" };
//    REQUIRE(qps_test::RunQuery("if i, q; variable v; Select q pattern i(\"count\", _, _)", *pkb_querier) == expected);
//    REQUIRE(qps_test::RunQuery("if i, q; variable v; Select q pattern i(\"cenY\", _, _)", *pkb_querier) == expected);
//  }
}
