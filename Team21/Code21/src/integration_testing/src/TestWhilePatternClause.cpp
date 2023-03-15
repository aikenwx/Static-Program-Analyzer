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
//    06      while (1 == 3) {
//            }
//    07      while (cenZ > cenY) {
//    08        print(count);
//            }
//    09      while (count == cenZ) {
//            }
//    10      while (count > 1) {
//            }

TEST_CASE("QPS can work with different combinations of while pattern") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = { "count", "cenX", "cenY", "cenZ" };
  data[qps::DesignEntity::CONSTANT] = { "1", "2", "4", "5", "6" };
  data[qps::DesignEntity::ASSIGN] = { "1", "2", "3", "4", "5" };
  data[qps::DesignEntity::PRINT] = { "8" };
  data[qps::DesignEntity::WHILE] = { "6", "7", "9", "10" };

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  QueryFacade* pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddStatementModifies({ {1, "count"}, {2, "count"}, {3, "cenX"}, {4, "cenY"}, {5, "cenZ"} });
  pkb_helper.AddStatementUses({ {2, "count"}, {4, "count"}, {4, "cenX"}, {7, "cenY"}, {7, "cenZ"}, {8, "count"}, {9,"count"}, {9, "cenZ"},
    {10, "count"} });


  pkb_helper.AddPostFixExpressions({ {1, R"("1""2"+)"},
                                    {2, R"("count""1"+)"},
                                    {3, R"("2""4"*"5""6"*+)"},
                                    {4, R"("2""count"*"1"+"2""cenX"*+)"},
                                    {5, R"("1""2"%"4""2"++)"} });

  pkb_helper.AddWhileStatementConditionVariable({ {7, R"(cenZ)"},
                                  {7, R"(cenY)"},
                                  {9, R"(count)"},
                                  {9, R"(cenZ)"},
                                  {10, R"(count)"} });

  SECTION("Can check for while statements with synonym") {
    std::unordered_set<std::string> expected{ "7", "9", "10" };
    REQUIRE(qps_test::RunQuery("while w; variable v; Select w pattern w(v, _)", *pkb_querier) == expected);
  }

  SECTION("Can retrive while statements with underscore") {
    std::unordered_set<std::string> expected{ "7", "9", "10" };
    REQUIRE(qps_test::RunQuery("while w; variable v; Select w pattern w(_, _)", *pkb_querier) == expected);
  }

  SECTION("Can retrieve while statements with quoted identifier") {
    std::unordered_set<std::string> expected{ "9", "10" };
    REQUIRE(qps_test::RunQuery("while w; variable v; Select w pattern w(\"count\", _)", *pkb_querier) == expected);
    expected = { "7" };
    REQUIRE(qps_test::RunQuery("while w; variable v; Select w pattern w(\"cenY\", _)", *pkb_querier) == expected);
  }

  SECTION("Can check for while statements with synonym and return different while synonym") {
    std::unordered_set<std::string> expected{ "6", "7", "9", "10" };
    REQUIRE(qps_test::RunQuery("while w, q; variable v; Select q pattern w(v, _)", *pkb_querier) == expected);
  }

  SECTION("Can retrive while statements with underscore and return different while synonym") {
    std::unordered_set<std::string> expected{ "6", "7", "9", "10" };
    REQUIRE(qps_test::RunQuery("while w, q; variable v; Select q pattern w(_, _)", *pkb_querier) == expected);
  }

  SECTION("Can retrieve while statements with quoted identifier and return different while synonym") {
    std::unordered_set<std::string> expected{ "6", "7", "9", "10" };
    REQUIRE(qps_test::RunQuery("while w, q; variable v; Select q pattern w(\"count\", _)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("while w, q; variable v; Select q pattern w(\"cenY\", _)", *pkb_querier) == expected);
  }
}
