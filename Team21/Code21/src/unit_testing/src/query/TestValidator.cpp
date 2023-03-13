#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "query_validation/SemanticValidator.h"
#include "query_validation/SyntacticValidator.h"
#include "QueryHelper.h"

using namespace qps;

TEST_CASE("Semantic validation for queries") {
    using Catch::Matchers::ContainsSubstring;

    SECTION("Duplicate declaration") {
        std::string dupeInput("stmt s; procedure s; variable v; assign a; Select s such that Parent (s, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), ContainsSubstring("There is duplicate declaration found for s"));
    }

    SECTION("No duplicate declaration when capitals are used") {
        std::string dupeInput("stmt s; procedure p; variable P; assign S; Select P");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE(validator.validateQuery() == true);
    }

    SECTION("The synonym for select clause is not previously declared") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select S");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is missing declaration in Select clause for synonym S"));
    }

    SECTION("The synonym for such that clause is not previously declared") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent(w, s)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. There is missing declaration in SuchThat clause"));
    }

    SECTION("The synonym for assignPattern clause is not previously declared") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s pattern a (New, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is missing declaration in Pattern clause for argument 1"));
    }

    SECTION("The synonym for assignPattern clause is not previously declared and with double clause") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent(s, _) pattern a (New, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. There is missing declaration in Pattern clause for argument 1"));
    }

    SECTION("check for WildCard as firstArg in Modifies") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Modifies(_, v)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. There is wild card as first argument in Modifies"));
    }

    SECTION("check for WildCard as firstArg in Uses") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Uses(_, v)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. There is wild card as first argument in Uses"));
    }
    
    SECTION("check pattern clause has proper synonym") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select a pattern p (v, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            Contains("Semantic error. Invalid syntax for pattern with synonym: p"));
    }

    SECTION("check that pattern clause if have synonym, it is variable") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s pattern a (c, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. Wrong design entity type for pattern argument 1"));
    }

    SECTION("check that parent relationship have synonym of correct entity type") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent(p, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. Wrong design entity type for Parent"));
    }

    SECTION("check that parentT relationship have synonym of correct entity type") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent* (p, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. Wrong design entity type for Parent*"));
    }

    SECTION("check that modifies relationship have synonym of correct entity type") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Modifies (c, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. Wrong design entity type for Modifies"));
    }

    SECTION("check that uses relationship have synonym of correct entity type") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Uses (c, _)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. Wrong design entity type for Uses"));
    }

    SECTION("check that modifies relationship cannot have non variable synonym for second argument") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Modifies (p, c)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. Wrong design entity type for Modifies(P)"));
    }

    SECTION("check that uses relationship cannot have non variable synonym for second argument") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Uses (p, s)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SemanticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(),
            ContainsSubstring("Semantic error. Wrong design entity type for Uses(P)"));
    }

    SECTION("check that calls relationship cannot have non variable synonym for first argument") {
      std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Calls (v, p)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        ContainsSubstring("Semantic error. Wrong design entity type for Calls"));
    }

    SECTION("check that callsT relationship cannot have non variable synonym for first argument") {
      std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Calls (p, c)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        ContainsSubstring("Semantic error. Wrong design entity type for Calls"));
    }

    SECTION("check that calls relationship cannot have non variable synonym for second argument") {
      std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Calls* (a, p)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        ContainsSubstring("Semantic error. Wrong design entity type for Calls*"));
    }

    SECTION("check that callsT relationship cannot have non variable synonym for second argument") {
      std::string dupeInput("stmt s; procedure p; variable v; assign a; constant c; Select s such that Calls* (p, a)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        ContainsSubstring("Semantic error. Wrong design entity type for Calls*"));
    }

    SECTION("check correct select clause with tuple and attrRef as element") {
      std::string dupeInput("stmt s; procedure p; variable P; assign S; Select <s.stmt#, p.procName, P>");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE(validator.validateQuery() == true);
    }

    SECTION("check that attrName is acceptable with given synonym") {
      std::string dupeInput("stmt s; procedure p; variable P; assign S; Select <s.value, p.procName, P>");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. Attribute name is invalid with synonym s"));
    }

    SECTION("check correct attrRef with synonym BOOLEAN") {
      std::string dupeInput("stmt s; procedure BOOLEAN; variable P; assign S; Select <s.stmt#, BOOLEAN.procName, P>");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE(validator.validateQuery() == true);
    }

    SECTION("check synonym BOOLEAN have declaration") {
      std::string dupeInput("stmt s; variable P; assign S; Select <s.stmt#, BOOLEAN, P>");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. There is missing declaration in Select clause for synonym BOOLEAN"));
    }

    SECTION("check synonym BOOLEAN have declaration") {
      std::string dupeInput("variable P; assign S; Select <s.stmt#, S, P>");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. There is missing declaration in Select clause for synonym (in AttrRef) s"));
    }

    SECTION("check synonym BOOLEAN in attrRef have declaration") {
      std::string dupeInput("stmt s; variable P; assign S; Select <s.stmt#, BOOLEAN.varName, P>");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. There is missing declaration in Select clause for synonym (in AttrRef) BOOLEAN"));
    }

    SECTION("check that withRef in WithClause have declaration") {
      std::string dupeInput("stmt s; procedure p; Select s with s.stmt#=c.value and p.procName=25");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. There is missing declaration for synonym (in WithRef) c"));
    }

    SECTION("check that attrName is acceptable with synonym in WithClause") {
      std::string dupeInput("stmt s; constant c; Select s with s.stmt#=c.value and c.stmt#=25");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. Attribute name is invalid with synonym c"));
    }

    SECTION("check that correct comparison of withRef in WithClause") {
      std::string dupeInput("stmt s; constant c; procedure p; Select s with s.stmt#=c.value and p.procName=25");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. Comparison of different type of attribute in with clause"));
    }

    SECTION("check correct pattern with if/assign/while synonym") {
      std::string dupeInput("if i; assign a; while w; Select <a.stmt#, w, i> pattern a(_,_) and w(_,_) and i(_,_,_)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE(validator.validateQuery() == true);
    }

    SECTION("check incorrect pattern with not if/assign/while synonym") {
      std::string dupeInput("if i; assign a; stmt w; Select <a.stmt#, w, i> pattern a(_,_) and w(_,_) and i(_,_,_)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. Invalid syntax for pattern with synonym: w"));
    }

    SECTION("check pattern if/assign/while synonym have declaration") {
      std::string dupeInput("if i; while w; Select <w, i> pattern a(_,_) and w(_,_) and i(_,_,_)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SemanticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Semantic error. There is missing declaration in Select clause for a"));
    }

}

TEST_CASE("Syntactic validation for queries") {
    using Catch::Matchers::ContainsSubstring;

    SECTION("check parent cannot have quoted identifier as argument 2") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Parent (s, \"hoho\")");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE(dupeQuery.getSuchThatClause()[0].getArg2().index() == 3);
        REQUIRE_THROWS_WITH(validator.validateQuery(), ContainsSubstring("Syntactic error. The argument is not of correct ref type for Parent"));
    }

    SECTION("check follow* cannot have quoted identifier as argument 2") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Follows* (s, \"hijuyg\")");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), ContainsSubstring("Syntactic error. The argument is not of correct ref type for Follows*"));
    }

    SECTION("check follow cannot have quoted identifier as argument 1") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Follows (\"h\", s)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), ContainsSubstring("Syntactic error. The argument is not of correct ref type for Follows"));
    }

    SECTION("check modifies cannot have statement number as argument 2") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s such that Modifies (s, 100)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), ContainsSubstring("Syntactic error. The argument is not of correct ref type for Modifies(S)"));
    }

    SECTION("check uses cannot have statement number as argument 2") {
        std::string dupeInput("stmt s; procedure k; variable v; assign a; Select s such that Uses (k, 100)");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), ContainsSubstring("Syntactic error. The argument is not of correct ref type for Uses(P)"));
    }

    SECTION("check AssignPattern have correct ref types") {
        std::string dupeInput("stmt s; procedure p; variable v; assign a; Select s pattern a (1000, \"x+Y*Z\")");
        qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
        qps::SyntacticValidator validator(dupeQuery);
        REQUIRE_THROWS_WITH(validator.validateQuery(), ContainsSubstring("Syntactic error. The first argument is not of correct ref type for assign clause"));
    }

    SECTION("check that calls relationship cannot allow statement number for first argument") {
      std::string dupeInput("procedure pp; procedure px; Select pp such that Calls (1, px)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SyntacticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        ContainsSubstring("Syntactic error. The argument is not of correct ref type for Calls"));
    }

    SECTION("check that calls relationship cannot allow statement number for second argument") {
      std::string dupeInput("procedure pp; procedure px; Select pp such that Calls (pp, 14)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SyntacticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        ContainsSubstring("Syntactic error. The argument is not of correct ref type for Calls"));
    }

    SECTION("check that callsT relationship cannot allow statement number for first argument") {
      std::string dupeInput("procedure pp; procedure px; Select pp such that Calls* (12323, px)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SyntacticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        ContainsSubstring("Syntactic error. The argument is not of correct ref type for Calls*"));
    }

    SECTION("check that callsT relationship cannot allow statement number for second argument") {
      std::string dupeInput("procedure pp; procedure px; Select pp such that Calls* (pp, 145678)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SyntacticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        ContainsSubstring("Syntactic error. The argument is not of correct ref type for Calls*"));
    }

    SECTION("check that attrRef (attrName) is incorrect") {
      std::string dupeInput("stmt s; procedure p; variable P; assign S; Select <s.stmt, p.procName, P>");
      REQUIRE_THROWS_WITH(QueryHelper::buildQuery(dupeInput),
        Contains("Syntactic error. Invalid Attribute Name stmt"));
    }

    SECTION("check incorrect arg2 with while pattern") {
      std::string dupeInput("if i; assign a; while w; Select <a.stmt#, w, i> pattern a(_,_) and w(_,\"x\") and i(_,_,_)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SyntacticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Syntactic error. The second argument of if/while pattern cannot be expression"));
    }

    SECTION("check incorrect arg2 with if pattern") {
      std::string dupeInput("if i; assign a; while w; Select <a.stmt#, w, i> pattern a(_,_) and w(_,_) and i(_,\"x\",_)");
      qps::Query dupeQuery = QueryHelper::buildQuery(dupeInput);
      qps::SyntacticValidator validator(dupeQuery);
      REQUIRE_THROWS_WITH(validator.validateQuery(),
        Contains("Syntactic error. The second argument of if/while pattern cannot be expression"));
    }

    SECTION("check incorrect arg3 with if pattern") {
      std::string dupeInput("if i; assign a; while w; Select <a.stmt#, w, i> pattern a(_,_) and w(_,_) and i(_,_,\"x\")");
      REQUIRE_THROWS_WITH(QueryHelper::buildQuery(dupeInput),
        Contains("Syntactic error. Invalid Query Syntax. Expect (_) got (\")"));
    }
}
