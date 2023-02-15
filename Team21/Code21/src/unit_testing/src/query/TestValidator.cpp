#include "query_validation/SemanticValidator.h"
#include "query_validation/SyntacticValidator.h"
#include "QueryHelper.h"

#include "catch.hpp"

using namespace qps;

TEST_CASE("Semantic validation for queries") {
    using Catch::Matchers::Contains;

    SECTION("Duplicate declaration") {
        std::string dupeInput("stmt s; procedure s; variable v; assign a; Select s such that Parent (s, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("There is duplicate declaration found for s"));
    }

    SECTION("No duplicate declaration when capitals are used") {
        std::string dupeInput("stmt s; procedure p; variable P; assign S; Select P");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
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
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is missing declaration in SuchThat clause"));
    }

    SECTION("The synonym for assignPattern clause is not previously declared") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s pattern a (New, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is missing declaration in AssignPattern clause for argument 1"));
    }

    SECTION("The synonym for assignPattern clause is not previously declared and with double clause") {
        std::string
            dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent(s, _) pattern a (New, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is missing declaration in AssignPattern clause for argument 1"));
    }
}

TEST_CASE("Syntactic validation for queries") {
    using Catch::Matchers::Contains;

    SECTION("Duplic") {
        std::string dupeInput("stmt s; procedure s; variable v; assign a; Select s such that Parent (s, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("There is duplicate declaration found for s"));
    }
}
