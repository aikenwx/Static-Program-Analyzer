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
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent(s, _) pattern a (New, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is missing declaration in AssignPattern clause for argument 1"));
    }

    SECTION("check for WildCard as firstArg in Modifies") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Modifies(_, v)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is wild card as first argument in Modifies"));
    }

    SECTION("check for WildCard as firstArg in Uses") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Uses(_, v)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is wild card as first argument in Uses"));
    }

    SECTION("check pattern clause has proper assign synonym") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select a pattern p (New, _)");
        REQUIRE_THROWS_WITH(QueryHelper::buildQuery(dupeInput),
            Contains("Semantic error. Invalid syntax for pattern assign with synonym"));
    }


    SECTION("check that parent relationship have synonym of correct entity type") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent(p, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. Wrong design entity type for Parent"));
    }

    SECTION("check that parentT relationship have synonym of correct entity type") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent* (p, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. Wrong design entity type for Parent*"));
    }

    SECTION("check that modifies relationship have synonym of correct entity type") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Modifies (c, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. Wrong design entity type for Modifies"));
    }

    SECTION("check that uses relationship have synonym of correct entity type") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Uses (c, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. Wrong design entity type for Uses"));
    }

    SECTION("check that modifies relationship cannot have non variable synonym for second argument") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Modifies (p, c)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. Wrong design entity type for Modifies(P)"));
    }

    SECTION("check that uses relationship cannot have non variable synonym for second argument") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Uses (p, s)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. Wrong design entity type for Uses(P)"));
    }
}

TEST_CASE("Syntactic validation for queries") {
    using Catch::Matchers::Contains;

    SECTION("check parent cannot have quoted identifier as argument 2") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent (s, \"hoho\")");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE(dupeQuery.getSuchThatClause()[0].getArg2().index() == 3);
        REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("Syntactic error. The argument is not of correct ref type for Parent"));
    }

    SECTION("check follow* cannot have quoted identifier as argument 2") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Follows* (s, \"hijuyg\")");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("Syntactic error. The argument is not of correct ref type for Follows*"));
    }

    SECTION("check follow cannot have quoted identifier as argument 1") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Follows (\"h\", s)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("Syntactic error. The argument is not of correct ref type for Follows"));
    }

    SECTION("check modifies cannot have statement number as argument 2") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Modifies (s, 100)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("Syntactic error. The argument is not of correct ref type for Modifies(S)"));
    }

    SECTION("check uses cannot have statement number as argument 2") {
        std::string dupeInput("stmt s; procedure k; variable v; assign a; Select s such that Uses (k, 100)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("Syntactic error. The argument is not of correct ref type for Uses(P)"));
    }

    SECTION("check AssignPattern have correct ref types") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s pattern a (1000, \"x+Y*Z\")");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), Contains("Syntactic error. The first argument is not of correct ref type for assign clause"));
    }
}
