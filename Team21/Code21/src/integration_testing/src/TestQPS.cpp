#include <utility>

#include "catch.hpp"
#include "PKB/PKB.h"
#include "query/ref.h"
#include "query/query.h"
#include "query/such_that_clause.h"
#include "query/declaration.h"
#include "query_evaluators/query_evaluator.h"
#include "PopulatePKBHelper.h"

using Data = std::unordered_map<qps::DesignEntity, std::unordered_set<std::string>>;

void PopulateEntities(PopulatePKBHelper &pkb_helper, Data &data) {
  pkb_helper.AddVariables(data[qps::DesignEntity::VARIABLE]);
  pkb_helper.AddConstants(data[qps::DesignEntity::CONSTANT]);
  pkb_helper.AddAssignments(data[qps::DesignEntity::ASSIGN]);
  pkb_helper.AddRead(data[qps::DesignEntity::READ]);
  pkb_helper.AddPrint(data[qps::DesignEntity::PRINT]);
  pkb_helper.AddIf(data[qps::DesignEntity::IF]);
  pkb_helper.AddWhile(data[qps::DesignEntity::WHILE]);
  pkb_helper.AddCalls(data[qps::DesignEntity::CALL]);
  pkb_helper.AddProcedures(data[qps::DesignEntity::PROCEDURE]);
}
std::unordered_set<std::string> RunSelect(qps::DesignEntity entity_type, std::string synonym, QueryFacade *pkb) {
  qps::Synonym s{std::move(synonym)};
  qps::Declaration dec{entity_type, s};
  qps::Query query{dec};
  qps::QueryEvaluator evaluator(query);
  auto var_result = evaluator.evaluateQuery(*pkb);
  return var_result;
}

std::unordered_set<std::string> RunSuchThat(qps::DesignEntity entity_type, std::string synonym,
                                            qps::Relationship relation, qps::Ref arg1, qps::Ref arg2,
                                            QueryFacade *pkb) {
  qps::Synonym s{std::move(synonym)};
  qps::Declaration dec{entity_type, s};
  std::vector<qps::Declaration> decl_lst = {dec};
  qps::SuchThatClause clause{relation, std::move(arg1), std::move(arg2), decl_lst};
  std::vector<qps::SuchThatClause> clauses = {clause};
  qps::Query query{decl_lst, clauses, {}, dec};
  qps::QueryEvaluator evaluator(query);
  auto var_result = evaluator.evaluateQuery(*pkb);
  return var_result;
}

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
TEST_CASE("QPS can work with select and such that parent clause") {
  Data data;
  data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY", "flag", "cenX", "cenY", "x", "y"};
  data[qps::DesignEntity::CONSTANT] = {"0", "1"};
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "3", "6", "7", "8", "11", "12", "13", "14"};
  data[qps::DesignEntity::READ] = {};
  data[qps::DesignEntity::CALL] = {"4", "9"};
  data[qps::DesignEntity::IF] = {"10"};
  data[qps::DesignEntity::WHILE] = {"5"};
  data[qps::DesignEntity::PRINT] = {};
  data[qps::DesignEntity::PROCEDURE] = {"computeCentroid", "readPoint"};

  PopulatePKBHelper pkb_helper;
  PopulateEntities(pkb_helper, data);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();
  PopulateFacade *pkb_populater = pkb_helper.GetPopulater();
  pkb_populater->storeParentRelationship(5, EntityType::WHILE_STATEMENT, 6, EntityType::ASSIGN_STATEMENT);
  pkb_populater->storeParentRelationship(5, EntityType::WHILE_STATEMENT, 7, EntityType::ASSIGN_STATEMENT);
  pkb_populater->storeParentRelationship(5, EntityType::WHILE_STATEMENT, 8, EntityType::ASSIGN_STATEMENT);
  pkb_populater->storeParentRelationship(5, EntityType::WHILE_STATEMENT, 9, EntityType::CALL_STATEMENT);

  pkb_populater->storeParentRelationship(10, EntityType::IF_STATEMENT, 11, EntityType::ASSIGN_STATEMENT);
  pkb_populater->storeParentRelationship(10, EntityType::IF_STATEMENT, 12, EntityType::ASSIGN_STATEMENT);
  pkb_populater->storeParentRelationship(10, EntityType::IF_STATEMENT, 13, EntityType::ASSIGN_STATEMENT);

  // variable h; Select h
  SECTION("QPS can retrieve all variables") {
    auto res = RunSelect(qps::DesignEntity::VARIABLE, "h", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::VARIABLE]);
  }

    // stmt s; Select s such that Parent(s, 7)
  SECTION("QPS can retieve statements that are parent of a statement") {
    qps::Ref arg1{qps::Synonym("s")};
    qps::Ref arg2{7};
    auto res = RunSuchThat(qps::DesignEntity::STMT, "s", qps::Relationship::Parent, arg1, arg2, pkb_querier);
    std::unordered_set<std::string> expected{"5"};
    REQUIRE(expected == res);
  }

    // stmt s; Select s such that Parent(_, s)
  SECTION("QPS can retrieve all statements that are a child") {
    qps::Ref arg1{qps::Underscore()};
    qps::Ref arg2{qps::Synonym("s")};
    auto res = RunSuchThat(qps::DesignEntity::STMT, "s", qps::Relationship::Parent, arg1, arg2, pkb_querier);
    std::unordered_set<std::string> expected{"6", "7", "8", "9", "11", "12", "13"};
    REQUIRE(expected == res);
  }

    // assign a; Select s such that Parent(_, a)
  SECTION("QPS can retrieve all assigments that are a child") {
    qps::Ref arg1{qps::Underscore()};
    qps::Ref arg2{qps::Synonym("a")};
    auto res = RunSuchThat(qps::DesignEntity::ASSIGN, "a", qps::Relationship::Parent, arg1, arg2, pkb_querier);
    std::unordered_set<std::string> expected{"6", "7", "8", "11", "12", "13"};
    REQUIRE(expected == res);
  }

    // call c; Select c such that Parent(_, c)
  SECTION("QPS can retrieve all calls that are a child") {
    qps::Ref arg1{qps::Underscore()};
    qps::Ref arg2{qps::Synonym("c")};
    auto res = RunSuchThat(qps::DesignEntity::CALL, "c", qps::Relationship::Parent, arg1, arg2, pkb_querier);
    std::unordered_set<std::string> expected{"9"};
    REQUIRE(expected == res);
  }
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
  data[qps::DesignEntity::PROCEDURE] = {"main", "calculate", "Evaluate"};

  PopulatePKBHelper pkb_helper;
  PopulateEntities(pkb_helper, data);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  SECTION("QPS can retrieve variables") {
    auto res = RunSelect(qps::DesignEntity::VARIABLE, "v", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::VARIABLE]);
  }

  SECTION("QPS can retrieve constants") {
    auto res = RunSelect(qps::DesignEntity::CONSTANT, "c", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::CONSTANT]);
  }

  SECTION("QPS can retrieve assignments") {
    auto res = RunSelect(qps::DesignEntity::ASSIGN, "a", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::ASSIGN]);
  }

  SECTION("QPS can retrieve prints") {
    auto res = RunSelect(qps::DesignEntity::PRINT, "p", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::PRINT]);
  }

  SECTION("QPS can retrieve ifs") {
    auto res = RunSelect(qps::DesignEntity::IF, "f", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::IF]);
  }

  SECTION("QPS can retrieve whiles") {
    auto res = RunSelect(qps::DesignEntity::WHILE, "w", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::WHILE]);
  }

  SECTION("QPS can retrieve calls") {
    auto res = RunSelect(qps::DesignEntity::CALL, "c", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::CALL]);
  }

  SECTION("QPS can retrieve procedures") {
    auto res = RunSelect(qps::DesignEntity::PROCEDURE, "p", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::PROCEDURE]);
  }

  SECTION("QPS can retrieve reads") {
    auto res = RunSelect(qps::DesignEntity::READ, "r", pkb_querier);
    REQUIRE(res == data[qps::DesignEntity::READ]);
  }

  SECTION("QPS can retrieve statements") {
    std::unordered_set<std::string> stmts;
    stmts.insert(data[qps::DesignEntity::ASSIGN].begin(), data[qps::DesignEntity::ASSIGN].end());
    stmts.insert(data[qps::DesignEntity::READ].begin(), data[qps::DesignEntity::READ].end());
    stmts.insert(data[qps::DesignEntity::PRINT].begin(), data[qps::DesignEntity::PRINT].end());
    stmts.insert(data[qps::DesignEntity::IF].begin(), data[qps::DesignEntity::IF].end());
    stmts.insert(data[qps::DesignEntity::WHILE].begin(), data[qps::DesignEntity::WHILE].end());
    stmts.insert(data[qps::DesignEntity::CALL].begin(), data[qps::DesignEntity::CALL].end());

    auto res = RunSelect(qps::DesignEntity::STMT, "r", pkb_querier);
    REQUIRE(res == stmts);
  }
}
