#include <utility>

#include "catch.hpp"
#include "query/query.h"
#include "query_preprocess/query_tokenizer.h"
#include "query_preprocess/query_parser.h"

namespace qps {

TEST_CASE("Parser: One Declaration, one synonym") {
	std::vector<std::string> tokens{ "variable", "v", ";", "Select", "v" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "v");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "v");
}

TEST_CASE("Parser: One Declarations, two synonyms") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "Select", "v" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "v");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
}

TEST_CASE("Parser: Two Declarations, two synonyms") {
	std::vector<std::string> tokens{ "variable", "u", ";", "procedure", "p", ";", "Select", "p" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "p");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "p");
}

TEST_CASE("Parser: Two Declarations, four synonyms") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ",", "q", ";", "Select", "q" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "q");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "p");
	REQUIRE(decl_list[3].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[3].getSynonym().getSynonym() == "q");
}

TEST_CASE("Parser: pattern(syn,expr)") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "u", ",", "\"", "x + y", "\"",")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "a");
	std::vector<PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == PatternClause(Synonym("a"), Synonym("u"), Expression(false, "x + y")));
}

TEST_CASE("Parser: pattern(_,partial_expr)") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "u", ",", "_", "\"", "x + y", "\"", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "a");
	std::vector<PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == PatternClause(Synonym("a"), Synonym("u"), Expression(true, "x + y")));
}

TEST_CASE("Parser: pattern(quoted_id,_)") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "\"", "y", "\"", ",", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "a");
	std::vector<PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == PatternClause(Synonym("a"), QuotedIdentifier("y"), Underscore()));
}

TEST_CASE("Parser: follows(syn,syn)") {
	std::vector<std::string> tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Follows", "(", "a", ",", "b", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "b");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::Follows, Synonym("a"), Synonym("b"), decl_list));
}

TEST_CASE("Parser: follows*(_,_)") {
	std::vector<std::string> tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Follows*", "(", "_", ",", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "b");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::FollowsT, Underscore(), Underscore(), decl_list));
}

TEST_CASE("Parser: parent(syn,stmtNo)") {
	std::vector<std::string> tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Parent", "(", "a", ",", "2", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "b");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::Parent, Synonym("a"), 2, decl_list));
}

TEST_CASE("Parser: parentT(stmtNo,stmtNo)") {
	std::vector<std::string> tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Parent*", "(", "1", ",", "2", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "b");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::ParentT, 1, 2, decl_list));
}

TEST_CASE("Parser: modifiesS(stmtNo,syn)") {
	std::vector<std::string> tokens{ "variable", "u", ";", "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Modifies", "(", "1", ",", "u", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::ModifiesS, 1, Synonym("u"), decl_list));
}

TEST_CASE("Parser: modifiesP(syn,quoted_id)") {
	std::vector<std::string> tokens{ "variable", "u", ";", "procedure", "a", ",", "b", ";", "Select", "a", "such", "that", "Modifies", "(", "a", ",", "\"", "x", "\"", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::ModifiesP, Synonym("a"), QuotedIdentifier("x"), decl_list));
}

TEST_CASE("Parser: usesS(syn,_)") {
	std::vector<std::string> tokens{ "variable", "u", ";", "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Uses", "(", "a", ",", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::UsesS, Synonym("a"), Underscore(), decl_list));
}

TEST_CASE("Parser: usesP(quoted_id,_)") {
	std::vector<std::string> tokens{ "variable", "u", ";", "procedure", "a", ",", "b", ";", "Select", "a", "such", "that", "Uses", "(", "\"", "x", "\"", ",", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::UsesP, QuotedIdentifier("x"), Underscore(), decl_list));
}

TEST_CASE("Parser: pattern + such that") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ";", "assign", "a", ";", "Select",
		"p", "such", "that", "Modifies", "(", "p", ",", "u", ")", "pattern", "a", "(", "\"", "y", "\"", ",", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "p");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "p");
	REQUIRE(decl_list[3].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[3].getSynonym().getSynonym() == "a");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::ModifiesP, Synonym("p"), Synonym("u"), decl_list));
	std::vector<PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == PatternClause(Synonym("a"), QuotedIdentifier("y"), Underscore()));
}
}
