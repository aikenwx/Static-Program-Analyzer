#include <utility>

#include "catch.hpp"

#include "PopulatePKBHelper.h"
#include "QueryTestUtilities.h"
#include "query/design_entity.h"

//Sample program
//procedure computeCentroid {
//    01      count = 0;
//    02      cenX = 0;
//    03      cenY = 0;
//    04      call readPoint;
//    05      while ((x != 0) && (y != -1)) {
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
//    15      read z;
//    16      print flag
//}

TEST_CASE("QPS can work with only select clause") {
  PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY", "flag", "cenX", "cenY", "x", "y", "z"};
  data[qps::DesignEntity::CONSTANT] = {"0", "1", "-1"};
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "3", "6", "7", "8", "11", "12", "13", "14"};
  data[qps::DesignEntity::READ] = {"15"};
  data[qps::DesignEntity::CALL] = {"4", "9"};
  data[qps::DesignEntity::IF] = {"10"};
  data[qps::DesignEntity::WHILE] = {"5"};
  data[qps::DesignEntity::PRINT] = {"16"};
  data[qps::DesignEntity::PROCEDURE] = {"computeCentroid", "readPoint"};

  PopulatePKBHelper pkb_helper;

  pkb_helper.PopulateEntities(data);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();


  SECTION("QPS can retrieve all variables") {
    REQUIRE(RunQuery("variable v; Select v", *pkb_querier) == data[qps::DesignEntity::VARIABLE]);
  }

  SECTION("QPS can retrieve all variables when synonym letter is changed") {
    REQUIRE(RunQuery("variable h; Select h", *pkb_querier) == data[qps::DesignEntity::VARIABLE]);
  }

  SECTION("QPS can retrieve all constants") {
    REQUIRE(RunQuery("constant c; Select c", *pkb_querier) == data[qps::DesignEntity::CONSTANT]);
  }

  SECTION("QPS can retrieve all assigns") {
    REQUIRE(RunQuery("assign a; Select a", *pkb_querier) == data[qps::DesignEntity::ASSIGN]);
  }

  SECTION("QPS can retrieve all ifs") {
    REQUIRE(RunQuery("if ifs; Select ifs", *pkb_querier) == data[qps::DesignEntity::IF]);
  }

  SECTION("QPS can retrieve all whiles") {
    REQUIRE(RunQuery("while WHILES; Select WHILES", *pkb_querier) == data[qps::DesignEntity::WHILE]);
  }

  SECTION("QPS can retrieve all procedures") {
    REQUIRE(RunQuery("procedure p; Select p", *pkb_querier) == data[qps::DesignEntity::PROCEDURE]);
  }

  SECTION("QPS can retrieve all prints") {
    REQUIRE(RunQuery("print pn; Select pn", *pkb_querier) == data[qps::DesignEntity::PRINT]);
  }

  SECTION("QPS can retrieve all calls") {
    REQUIRE(RunQuery("call c; Select c", *pkb_querier) == data[qps::DesignEntity::CALL]);
  }

  SECTION("QPS can retrieve all reads") {
    REQUIRE(RunQuery("read r; Select r", *pkb_querier) == data[qps::DesignEntity::READ]);
  }

  SECTION("QPS can retrieve statements") {
    std::unordered_set<std::string> stmts;
    stmts.insert(data[qps::DesignEntity::ASSIGN].begin(), data[qps::DesignEntity::ASSIGN].end());
    stmts.insert(data[qps::DesignEntity::READ].begin(), data[qps::DesignEntity::READ].end());
    stmts.insert(data[qps::DesignEntity::PRINT].begin(), data[qps::DesignEntity::PRINT].end());
    stmts.insert(data[qps::DesignEntity::IF].begin(), data[qps::DesignEntity::IF].end());
    stmts.insert(data[qps::DesignEntity::WHILE].begin(), data[qps::DesignEntity::WHILE].end());
    stmts.insert(data[qps::DesignEntity::CALL].begin(), data[qps::DesignEntity::CALL].end());

    REQUIRE(RunQuery("stmt s; Select s", *pkb_querier) == stmts);
  }
}


