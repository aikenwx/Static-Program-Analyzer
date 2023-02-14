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
#include "query/validation/SemanticValidator.h"
#include "query_preprocess/query_parser.h"
#include "query_preprocess/query_tokenizer.h"

qps::Query buildQuery(std::string str) {
  std::string dupeInput(str);
  qps::QueryTokenizer tokenizer(dupeInput);
  std::vector<std::string> tokenList = tokenizer.tokenize();
  qps::QueryParser parser(tokenList);
  return parser.parse();
}

TEST_CASE("Queries can be built from parsing and tokenising from inputs") {
  SECTION("Can be built with single select clause only") {
    REQUIRE_NOTHROW(buildQuery("stmt s; Select s"));
  }

  SECTION("Can be built with single such that clause") {
    REQUIRE_NOTHROW(buildQuery("stmt s; Select s such that Modifies(s, _)"));
  }

  SECTION("Can be built with single assign pattern clause") {
    REQUIRE_NOTHROW(buildQuery("stmt s; assign a; Select s pattern a (s, _)"));
  }

  SECTION("Can be built with one such that and assign pattern clause") {
    REQUIRE_NOTHROW(buildQuery("stmt s; assign a; Select s such that Modifies(s, _) pattern a (s, _)"));
    REQUIRE_NOTHROW(buildQuery("stmt s; assign a; Select s pattern a (s, _) such that Modifies(s, _)"));
  }
}

TEST_CASE("Semantic validation for queries") {
  using Catch::Matchers::Contains;

  SECTION("Duplicate declaration") {
    std::string dupeInput("stmt s; procedure s; variable v; assign a; Select s such that Parent (s, _)");
    qps::Query dupeQuery = buildQuery(dupeInput);
    qps::SemanticValidator validator(dupeQuery);
    REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("There is duplicate declaration found for s"));
  }

  SECTION("No duplicate declaration when capitals are used") {
    std::string dupeInput("stmt s; procedure p; variable P; assign S; Select P");
    qps::Query dupeQuery = buildQuery(dupeInput);
    qps::SemanticValidator validator(dupeQuery);
    REQUIRE(validator.validateQuery() == true);
  }

  SECTION("The synonym for select clause is not previously declared") {
    std::string dupeInput("stmt s; procedure p; variable v; assign a; Select S");
    qps::QueryTokenizer tokenizer(dupeInput);
    std::vector<std::string> tokenList = tokenizer.tokenize();
    qps::QueryParser parser(tokenList);
    REQUIRE_THROWS_WITH(parser.parse(),
                        Contains("Semantic error. There is missing declaration in Select clause for S"));
  }

  SECTION("The synonym for such that clause is not previously declared") {
    std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent(w, s)");
    qps::Query dupeQuery = buildQuery(dupeInput);
    qps::SemanticValidator validator(dupeQuery);
    REQUIRE_THROWS_WITH(validator.validateQuery(),
                        Contains("Semantic error. There is missing declaration in SuchThat clause"));
  }

  SECTION("The synonym for assignPattern clause is not previously declared") {
    std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s pattern a (New, _)");
    qps::Query dupeQuery = buildQuery(dupeInput);
    qps::SemanticValidator validator(dupeQuery);
    REQUIRE_THROWS_WITH(validator.validateQuery(),
                        Contains("Semantic error. There is missing declaration in AssignPattern clause for argument 1"));
  }

  SECTION("The synonym for assignPattern clause is not previously declared and with double clause") {
    std::string
        dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent(s, _) pattern a (New, _)");
    qps::Query dupeQuery = buildQuery(dupeInput);
    qps::SemanticValidator validator(dupeQuery);
    REQUIRE_THROWS_WITH(validator.validateQuery(),
                        Contains("Semantic error. There is missing declaration in AssignPattern clause for argument 1"));
  }
}
