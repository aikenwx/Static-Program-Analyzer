#include <utility>

#include "catch.hpp"
#include "PKB/PKB.h"
#include "query/ref.h"
#include "query/query.h"
#include "query/such_that_clause.h"
#include "query/declaration.h"
#include "query_evaluators/query_evaluator.h"
#include "query_evaluators/QPS.h"
#include "PopulatePKBHelper.h"
#include "query_validation/SemanticValidator.h"
#include "query_preprocess/query_parser.h"
#include "query_preprocess/query_tokenizer.h"

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

std::unordered_set<std::string> RunQuery(std::string query_str, QueryFacade &pkb) {
  std::list<std::string> results;
  qps::QPS::evaluate(query_str, results, pkb);
  std::unordered_set<std::string> result_set(results.begin(), results.end());
  return result_set;
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

  pkb_populater->storeStatementModifiesVariableRelationship(6, EntityType::ASSIGN_STATEMENT, "count");
  pkb_populater->storeStatementModifiesVariableRelationship(7, EntityType::ASSIGN_STATEMENT, "cenX");
  pkb_populater->storeStatementModifiesVariableRelationship(7, EntityType::ASSIGN_STATEMENT, "cenY");
  pkb_populater->storeStatementModifiesVariableRelationship(12, EntityType::ASSIGN_STATEMENT, "cenX");
  pkb_populater->storeStatementModifiesVariableRelationship(13, EntityType::ASSIGN_STATEMENT, "cenY");




  SECTION("QPS can retrieve all variables") {
    REQUIRE(RunQuery("variable h; Select h", *pkb_querier) == data[qps::DesignEntity::VARIABLE]);
  }

  SECTION("QPS can retrieve variables modified in a single staement") {
    std::unordered_set<std::string> expected{"count"};
    REQUIRE(RunQuery("variable v; Select v such that Modifies(6, v)", *pkb_querier) == expected);
  }

  SECTION("QPS can retrieve all variables modified in a assign a statement") {
    std::unordered_set<std::string> expected{"count", "cenX", "cenY"};
    REQUIRE(RunQuery("variable v; assign a; Select v such that Modifies(a, v)", *pkb_querier) == expected);
  }

  SECTION("QPS can retrieve statements that are parent of a statement") {
    std::unordered_set<std::string> expected{"5"};
    REQUIRE(RunQuery("stmt s; Select s such that Parent(s, 7)", *pkb_querier) == expected);
  }

  SECTION("QPS can retrieve all statements that are a child") {
    std::unordered_set<std::string> expected{"6", "7", "8", "9", "11", "12", "13"};
    REQUIRE(RunQuery("stmt s; Select s such that Parent(_, s)", *pkb_querier) == expected);
  }

  SECTION("QPS can retrieve all assigments that are a child") {
    std::unordered_set<std::string> expected{"6", "7", "8", "11", "12", "13"};
    REQUIRE(RunQuery("assign a; Select a such that Parent(_, a)", *pkb_querier) == expected);
  }

  SECTION("QPS can retrieve all calls that are a child") {
    std::unordered_set<std::string> expected{"9"};
    REQUIRE(RunQuery("call c; Select c such that Parent(_, c)", *pkb_querier) == expected);
  }
}

TEST_CASE("QPS parse and can retrieve design entities") {
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
    REQUIRE(RunQuery("variable h; Select h", *pkb_querier) == data[qps::DesignEntity::VARIABLE]);
    REQUIRE(RunQuery("variable v; Select v", *pkb_querier) == data[qps::DesignEntity::VARIABLE]);

  }

  SECTION("QPS can retrieve constants") {
    REQUIRE(RunQuery("constant c; Select c", *pkb_querier) == data[qps::DesignEntity::CONSTANT]);
  }

  SECTION("QPS can retrieve assignments") {
    REQUIRE(RunQuery("assign a; Select a", *pkb_querier) == data[qps::DesignEntity::ASSIGN]);
  }

  SECTION("QPS can retrieve prints") {
    REQUIRE(RunQuery("print p; Select p", *pkb_querier) == data[qps::DesignEntity::PRINT]);
  }

  SECTION("QPS can retrieve ifs") {
    REQUIRE(RunQuery("if ifs; Select ifs", *pkb_querier) == data[qps::DesignEntity::IF]);
  }

  SECTION("QPS can retrieve whiles") {
    REQUIRE(RunQuery("while whiles; Select whiles", *pkb_querier) == data[qps::DesignEntity::WHILE]);
  }

  SECTION("QPS can retrieve calls") {
    REQUIRE(RunQuery("call calls; Select calls", *pkb_querier) == data[qps::DesignEntity::CALL]);
  }

  SECTION("QPS can retrieve procedures") {
    REQUIRE(RunQuery("procedure procs; Select procs", *pkb_querier) == data[qps::DesignEntity::PROCEDURE]);
  }

  SECTION("QPS can retrieve reads") {
    REQUIRE(RunQuery("read r; Select r", *pkb_querier) == data[qps::DesignEntity::READ]);

  }

  SECTION("QPS can get semantic error") {
    std::unordered_set<std::string> error = {"SemanticError"};
    REQUIRE(RunQuery("read r; Select a", *pkb_querier) == error);
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
