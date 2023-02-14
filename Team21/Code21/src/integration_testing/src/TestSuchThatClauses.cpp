#include <utility>

#include "catch.hpp"

#include "PopulatePKBHelper.cpp"
#include "QPSUtilities.h"
#include "query/design_entity.h"

//Sample program
//procedure computeCentroid {
//    01      count = 0;
//    02      cenX = 0;
//    03      cenY = 0;
//    04      call readPoint;
//    05      while ((x != 0) && (y != 0)) {
//      06          count = count + 1;
//      07          cenX = cenX + x;
//      08          cenY = cenY + y;
//      09          call readPoint;
//    }
//    10      if (count == 0) then {
//      11          flag = 1;
//    } else {
//      12          cenX = cenX / count;
//      13          cenY = cenY / count;
//    }
//    14      normSq = cenX * cenX + cenY * cenY;
//}
TEST_CASE("QPS can work with different combinations of parent") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY", "flag", "cenX", "cenY", "x", "y"};
  data[qps::DesignEntity::CONSTANT] = {"0", "1"};
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "3", "6", "7", "8", "11", "12", "13", "14"};
  data[qps::DesignEntity::READ] = {};
  data[qps::DesignEntity::CALL] = {"4", "9"};
  data[qps::DesignEntity::IF] = {"10"};
  data[qps::DesignEntity::WHILE] = {"5"};
  data[qps::DesignEntity::PRINT] = {};
  data[qps::DesignEntity::PROCEDURE] = {"computeCentroid", "readPoint"};

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddParent({{5, 6}, {5, 7}, {5, 8}, {5, 9}, {10, 11}, {10, 12}, {10, 13}});

  SECTION("Both Synonyms") {
    SECTION("Look for child statements under while") {
      std::unordered_set<std::string> expected{"6", "7", "8", "9"};
      REQUIRE(qps_test::RunQuery("stmt s; while w; Select s such that Parent(w, s)", *pkb_querier) == expected);
    }

    SECTION("Look for child statements under if") {
      std::unordered_set<std::string> expected{"11", "12", "13"};
      REQUIRE(qps_test::RunQuery("stmt s; if f; Select s such that Parent(f, s)", *pkb_querier) == expected);
    }

    SECTION("Look for child statements under any container stmt") {
      std::unordered_set<std::string> expected{"11", "12", "13", "6", "7", "8", "9"};
      REQUIRE(qps_test::RunQuery("stmt s, s1; Select s such that Parent(s1, s)", *pkb_querier) == expected);
    }

    SECTION("Look for call statements under any container stmt") {
      std::unordered_set<std::string> expected{"9"};
      REQUIRE(qps_test::RunQuery("stmt s1; call c; Select c such that Parent(s1, c)", *pkb_querier) == expected);
    }

    SECTION("Look for assign statements under any container stmt") {
      std::unordered_set<std::string> expected{"11", "12", "13", "6", "7", "8"};
      REQUIRE(qps_test::RunQuery("stmt s1; assign a; Select a such that Parent(s1, a)", *pkb_querier) == expected);
    }

    SECTION("Look for call statements under a while") {
      std::unordered_set<std::string> expected{"9"};
      REQUIRE(qps_test::RunQuery("while w; call c; Select c such that Parent(w, c)", *pkb_querier) == expected);
    }

    SECTION("Look for call statements under a if") {
      std::unordered_set<std::string> expected{};
      REQUIRE(qps_test::RunQuery("if f; call c; Select c such that Parent(f, c)", *pkb_querier) == expected);
    }

    SECTION("Look for statements under a assign") {
      std::unordered_set<std::string> expected{};
      REQUIRE(qps_test::RunQuery("assign a; call c; Select c such that Parent(a, c)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, wild card on left") {
    SECTION("All child statements") {
      std::unordered_set<std::string> expected{"11", "12", "13", "6", "7", "8", "9"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent(_, s)", *pkb_querier) == expected);
    }

    SECTION("All assign child statements") {
      std::unordered_set<std::string> expected{"11", "12", "13", "6", "7", "8"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent(_, a)", *pkb_querier) == expected);
    }

    SECTION("All print child statements") {
      std::unordered_set<std::string> expected{};
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Parent(_, pn)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("Statement on left") {
      std::unordered_set<std::string> expected{"5", "10"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent(s, _)", *pkb_querier) == expected);
    }

    SECTION("While on left") {
      std::unordered_set<std::string> expected{"5"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Parent(w, _)", *pkb_querier) == expected);
    }

    SECTION("If on left") {
      std::unordered_set<std::string> expected{"10"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Parent(f, _)", *pkb_querier) == expected);
    }

    SECTION("Non container stmt on left") {
      std::unordered_set<std::string> expected{};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent(a, _)", *pkb_querier) == expected);
      REQUIRE(qps_test::RunQuery("call c; Select c such that Parent(c, _)", *pkb_querier) == expected);
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Parent(pn, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"5"};
    REQUIRE(qps_test::RunQuery("while w; Select w such that Parent(w, 6)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("while w; Select w such that Parent(w, 9)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent(s, 9)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("while w; Select w such that Parent(w, 10)", *pkb_querier) == empty);

    expected = {"10"};
    REQUIRE(qps_test::RunQuery("if ifs; Select ifs such that Parent(ifs, 6)", *pkb_querier) == empty);
    REQUIRE(qps_test::RunQuery("if ifs; Select ifs such that Parent(ifs, 12)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent(s, 12)", *pkb_querier) == expected);
  }

  SECTION("Synonym on right, literal on left") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"6", "7", "8"};
    REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent(5, a)", *pkb_querier) == expected);

    expected = {"9"};
    REQUIRE(qps_test::RunQuery("call c; Select c such that Parent(5, c)", *pkb_querier) == expected);

    expected = {"11", "12", "13"};
    REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent(10, a)", *pkb_querier) == expected);
  }

  SECTION("Evaluates to true") {
    SECTION("Both wildcards") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Parent(_, _)", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("Both literals") {
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Parent(10, 11)", *pkb_querier)
                  == data[qps::DesignEntity::PROCEDURE]);

      REQUIRE(qps_test::RunQuery("constant c; Select c such that Parent(5, 8)", *pkb_querier)
                  == data[qps::DesignEntity::CONSTANT]);
    }
  }

  SECTION("Evaluates to false") {
    SECTION("Both literals") {
      std::unordered_set<std::string> empty{};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Parent(10, 9)", *pkb_querier)
                  == empty);
      REQUIRE(qps_test::RunQuery("constant c; Select c such that Parent(5, 4)", *pkb_querier)
                  == empty);
    }
  }
}
