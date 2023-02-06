#include <utility>

#include "catch.hpp"
#include "PKB/PKB.h"
#include "query/declaration.h"
#include "query_evaluators/select_evaluators/select_evaluator.h"
#include "PopulatePKBHelper.h"

using Data = std::unordered_map<qps::DesignEntity, std::unordered_set<std::string>>;

//template <typename T>
std::unordered_set<std::string> test(qps::DesignEntity entity_type, std::string synonym, QueryFacade *pkb) {
  qps::Synonym s{std::move(synonym)};
  qps::Declaration dec{entity_type, s};
  qps::SelectEvaluator select_clause_evaluator(dec);
  auto var_result = select_clause_evaluator.evaluate(*pkb).Extract(s);
  return var_result;
}

TEST_CASE("QPS can retrieve design entities") {
  Data data;
  data[qps::DesignEntity::VARIABLE] = {"x", "y", "z", "a"};
  data[qps::DesignEntity::CONSTANT] = {"20", "0", "1000", "-99"};
  data[qps::DesignEntity::ASSIGN] = {"1", "7", "11"};
  data[qps::DesignEntity::READ] = {"29", "56"};
  data[qps::DesignEntity::CALL] = {"4", "19", "23", "100"};
  data[qps::DesignEntity::IF] = {"3", "20", "50"};
  data[qps::DesignEntity::WHILE] = {"5", "17", "30"};
  data[qps::DesignEntity::PRINT] = {"4", "29", "22", "101"};
  data[qps::DesignEntity::PROCEDURE] = {"main", "calculate", "evaluate"};

  PopulatePKBHelper pkb_helper;
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();
  pkb_helper.AddVariables(data[qps::DesignEntity::VARIABLE]);
  pkb_helper.AddConstants(data[qps::DesignEntity::CONSTANT]);
  pkb_helper.AddAssignments(data[qps::DesignEntity::ASSIGN]);
  pkb_helper.AddRead(data[qps::DesignEntity::READ]);
  pkb_helper.AddPrint(data[qps::DesignEntity::PRINT]);
  pkb_helper.AddIf(data[qps::DesignEntity::IF]);
  pkb_helper.AddWhile(data[qps::DesignEntity::WHILE]);
  pkb_helper.AddCalls(data[qps::DesignEntity::CALL]);
  pkb_helper.AddProcedures(data[qps::DesignEntity::PROCEDURE]);

  SECTION("QPS can retrieve variables") {
    auto res = test(qps::DesignEntity::VARIABLE, "v", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::VARIABLE]);
  }

  SECTION("QPS can retrieve constants") {
    auto res = test(qps::DesignEntity::CONSTANT, "c", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::CONSTANT]);
  }

  SECTION("QPS can retrieve assignments") {
    auto res = test(qps::DesignEntity::ASSIGN, "a", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::ASSIGN]);
  }

  SECTION("QPS can retrieve prints") {
    auto res = test(qps::DesignEntity::PRINT, "p", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::PRINT]);
  }

  SECTION("QPS can retrieve ifs") {
    auto res = test(qps::DesignEntity::IF, "f", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::IF]);
  }

  SECTION("QPS can retrieve whiles") {
    auto res = test(qps::DesignEntity::WHILE, "w", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::WHILE]);
  }

  SECTION("QPS can retrieve calls") {
    auto res = test(qps::DesignEntity::CALL, "c", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::CALL]);
  }

  SECTION("QPS can retrieve procedures") {
    auto res = test(qps::DesignEntity::PROCEDURE, "p", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::PROCEDURE]);
  }

  SECTION("QPS can retrieve reads") {
    auto res = test(qps::DesignEntity::READ, "r", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::READ]);
  }

  SECTION("QPS can retrieve statements") {
    std::unordered_set < std::string > stmts;
    stmts.insert(data[qps::DesignEntity::ASSIGN].begin(), data[qps::DesignEntity::ASSIGN].end());
    stmts.insert(data[qps::DesignEntity::READ].begin(), data[qps::DesignEntity::READ].end());
    stmts.insert(data[qps::DesignEntity::PRINT].begin(), data[qps::DesignEntity::PRINT].end());
    stmts.insert(data[qps::DesignEntity::IF].begin(), data[qps::DesignEntity::IF].end());
    stmts.insert(data[qps::DesignEntity::WHILE].begin(), data[qps::DesignEntity::WHILE].end());
    stmts.insert(data[qps::DesignEntity::CALL].begin(), data[qps::DesignEntity::CALL].end());

    auto res = test(qps::DesignEntity::STMT, "r", pkb_querier);
    REQUIRE(res == stmts);
  }
}
