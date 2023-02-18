#include <utility>

#include "catch.hpp"
#include "query/query.h"
#include "query_preprocess/query_tokenizer.h"
#include "query_preprocess/query_parser.h"

TEST_CASE("Parser: One Declaration, one synonym") {
	std::vector<std::string> tokens{ "variable", "v", ";", "Select", "v" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "v");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "v");
}

TEST_CASE("Parser: One Declarations, two synonyms") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "Select", "v" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "v");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
}

TEST_CASE("Parser: Two Declarations, two synonyms") {
	std::vector<std::string> tokens{ "variable", "u", ";", "procedure", "p", ";", "Select", "p" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "p");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "p");
}

TEST_CASE("Parser: Two Declarations, four synonyms") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ",", "q", ";", "Select", "q" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "q");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "p");
	REQUIRE(decl_list[3].getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(decl_list[3].getSynonym().getSynonym() == "q");
}

TEST_CASE("Parser: pattern(syn,expr)") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "u", ",", "\"", "x + y", "\"",")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "a");
	std::vector<qps::PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == qps::PatternClause(qps::Synonym("a"), qps::Synonym("u"), qps::Expression(false, "x + y")));
}

TEST_CASE("Parser: pattern(_,partial_expr)") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "u", ",", "_", "\"", "x + y", "\"", "_", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "a");
	std::vector<qps::PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == qps::PatternClause(qps::Synonym("a"), qps::Synonym("u"), qps::Expression(true, "x + y")));
}

TEST_CASE("Parser: pattern(quoted_id,_)") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "\"", "y", "\"", ",", "_", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "a");
	std::vector<qps::PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == qps::PatternClause(qps::Synonym("a"), qps::QuotedIdentifier("y"), qps::Underscore()));
}

TEST_CASE("Parser: follows(syn,syn)") {
	std::vector<std::string> tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Follows", "(", "a", ",", "b", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "b");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::Follows, qps::Synonym("a"), qps::Synonym("b"), decl_list));
}

TEST_CASE("Parser: follows*(_,_)") {
	std::vector<std::string> tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Follows*", "(", "_", ",", "_", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "b");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::FollowsT, qps::Underscore(), qps::Underscore(), decl_list));
}

TEST_CASE("Parser: parent(syn,stmtNo)") {
	std::vector<std::string> tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Parent", "(", "a", ",", "2", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "b");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::Parent, qps::Synonym("a"), 2, decl_list));
}

TEST_CASE("Parser: parentT(stmtNo,stmtNo)") {
	std::vector<std::string> tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Parent*", "(", "1", ",", "2", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "b");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::ParentT, 1, 2, decl_list));
}

TEST_CASE("Parser: modifiesS(stmtNo,syn)") {
	std::vector<std::string> tokens{ "variable", "u", ";", "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Modifies", "(", "1", ",", "u", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::ModifiesS, 1, qps::Synonym("u"), decl_list));
}

TEST_CASE("Parser: modifiesP(syn,quoted_id)") {
	std::vector<std::string> tokens{ "variable", "u", ";", "procedure", "a", ",", "b", ";", "Select", "a", "such", "that", "Modifies", "(", "a", ",", "\"", "x", "\"", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::ModifiesP, qps::Synonym("a"), qps::QuotedIdentifier("x"), decl_list));
}

TEST_CASE("Parser: usesS(syn,_)") {
	std::vector<std::string> tokens{ "variable", "u", ";", "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Uses", "(", "a", ",", "_", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::UsesS, qps::Synonym("a"), qps::Underscore(), decl_list));
}

TEST_CASE("Parser: usesP(quoted_id,_)") {
	std::vector<std::string> tokens{ "variable", "u", ";", "procedure", "a", ",", "b", ";", "Select", "a", "such", "that", "Uses", "(", "\"", "x", "\"", ",", "_", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "a");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::UsesP, qps::QuotedIdentifier("x"), qps::Underscore(), decl_list));
}

TEST_CASE("Parser: pattern + such that") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ";", "assign", "a", ";", "Select",
		"p", "such", "that", "Modifies", "(", "p", ",", "u", ")", "pattern", "a", "(", "\"", "y", "\"", ",", "_", ")" };
	qps::QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "p");
	std::vector<qps::Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == qps::DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == qps::DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "p");
	REQUIRE(decl_list[3].getDesignEntity() == qps::DesignEntity::ASSIGN);
	REQUIRE(decl_list[3].getSynonym().getSynonym() == "a");
	std::vector<qps::SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == qps::SuchThatClause(qps::Relationship::ModifiesP, qps::Synonym("p"), qps::Synonym("u"), decl_list));
	std::vector<qps::PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == qps::PatternClause(qps::Synonym("a"), qps::QuotedIdentifier("y"), qps::Underscore()));
}
