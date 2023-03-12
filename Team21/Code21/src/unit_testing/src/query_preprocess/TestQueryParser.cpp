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
	REQUIRE(patternClause[0] == PatternClause(Synonym("a"), Synonym("u"), Expression(false, "\"x\"\"y\"+")));
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
	//std::string expr = std::get<Expression>(patternClause[0].getArg2()).getExpression();
	//std::cout << expr;
	REQUIRE(patternClause[0] == PatternClause(Synonym("a"), Synonym("u"), Expression(true, "\"x\"\"y\"+")));
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

TEST_CASE("Parser: withClause integer") {
	std::vector<std::string> tokens{ "constant", "u", ";", "assign", "a", ",", "b", ";", "Select", "a",
		"with", "a", ".", "stmt#", "=", "25" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "a");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::CONSTANT);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<WithClause> withClause = query.getWithClause();
	REQUIRE(withClause[0] == WithClause(WithRef(AttrRef(Synonym("a"), AttrName::StmtNo)), WithRef(25)));
}

TEST_CASE("Parser: withClause name") {
	std::vector<std::string> tokens{ "variable", "u", ";", "assign", "a", ",", "b", ";", "Select", "b",
		"with", "u", ".", "varName", "=", "\"", "y", "\"" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "b");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "b");
	std::vector<WithClause> withClause = query.getWithClause();
	REQUIRE(withClause[0] == WithClause(WithRef(AttrRef(Synonym("u"), AttrName::VarName)), WithRef(QuotedIdentifier("y"))));
}

TEST_CASE("Parser: multi-and withClause + tuple Select") {
	std::vector<std::string> tokens{ "variable", "u", ";", "constant", "c", ";", "assign", "a", ";", "call", "b",
		";", "Select", "<", "b", ".", "stmt#", ",", "u", ">", "with", "u", ".", "varName", "=", "b", ".", "procName",
		"and", "c", ".", "value", "=", "a", ".", "stmt#" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<AttrRef>(select_tuple[0]) == AttrRef(Synonym("b"), AttrName::StmtNo));
	REQUIRE(std::get<Synonym>(select_tuple[1]).getSynonym() == "u");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::CONSTANT);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "c");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[3].getDesignEntity() == DesignEntity::CALL);
	REQUIRE(decl_list[3].getSynonym().getSynonym() == "b");
	std::vector<WithClause> withClause = query.getWithClause();
	REQUIRE(withClause[0] == WithClause(WithRef(AttrRef(Synonym("u"), AttrName::VarName)), WithRef(AttrRef(Synonym("b"), AttrName::ProcName))));
	REQUIRE(withClause[1] == WithClause(WithRef(AttrRef(Synonym("c"), AttrName::Value)), WithRef(AttrRef(Synonym("a"), AttrName::StmtNo))));
}

TEST_CASE("Parser: pattern + such that + BOOLEAN return") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ";", "assign", "a", ";", "Select",
		"BOOLEAN", "such", "that", "Modifies", "(", "p", ",", "u", ")", "pattern", "a", "(", "\"", "y", "\"", ",", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	REQUIRE(std::get<Boolean>(query.getSelectClause()) == Boolean());
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

TEST_CASE("Parser: pattern if") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "if", "i", ";", "Select", "i", "pattern", "i", "(", "\"", "y", "\"", ",", "_", ",", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "i");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::IF);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "i");
	std::vector<PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == PatternClause(Synonym("i"), QuotedIdentifier("y"), Underscore()));
}

TEST_CASE("Parser: BOOLEAN declared") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "if", "BOOLEAN", ";", "Select", "BOOLEAN", "pattern", "BOOLEAN", "(", "\"", "y", "\"", ",", "_", ",", "_", ")" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<Synonym>(select_tuple[0]).getSynonym() == "BOOLEAN");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::IF);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "BOOLEAN");
	std::vector<PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == PatternClause(Synonym("BOOLEAN"), QuotedIdentifier("y"), Underscore()));
}

TEST_CASE("Parser: multi-and pattern + such that") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ";", "assign", "a", ";", "while", "w", ";", "Select",
		"p", "such", "that", "Modifies", "(", "p", ",", "u", ")", "and", "Modifies", "(", "a", ",", "v", ")",
		"pattern", "a", "(", "\"", "y", "\"", ",", "_", ")", "and", "w", "(", "\"", "y", "\"", ",", "_", ")" };
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
	REQUIRE(decl_list[4].getDesignEntity() == DesignEntity::WHILE);
	REQUIRE(decl_list[4].getSynonym().getSynonym() == "w");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::ModifiesP, Synonym("p"), Synonym("u"), decl_list));
	REQUIRE(suchThatClause[1] == SuchThatClause(Relationship::ModifiesS, Synonym("a"), Synonym("v"), decl_list));
	std::vector<PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == PatternClause(Synonym("a"), QuotedIdentifier("y"), Underscore()));
	REQUIRE(patternClause[1] == PatternClause(Synonym("w"), QuotedIdentifier("y"), Underscore()));
}

TEST_CASE("Parser: full-use") {
	std::vector<std::string> tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ";", "constant", "c", ";", "assign", "a", ";", "call", "b",
		";", "while", "w", ";", "Select", "<", "a", ".", "stmt#", ",", "v", ">", "with", "u", ".", "varName", "=", "b", ".", "procName",
		"and", "c", ".", "value", "=", "a", ".", "stmt#", "such", "that", "Modifies", "(", "p", ",", "u", ")", "and", "Modifies", "(", "a", ",", "v", ")",
		"pattern", "a", "(", "\"", "y", "\"", ",", "_", ")", "and", "w", "(", "\"", "x", "\"", ",", "_", ")", "with", "v", ".", "varName", "=", "\"", "x", "\"" };
	QueryParser parser(tokens);
	auto query = parser.parse();
	std::vector<Element> select_tuple = std::get<std::vector<Element>>(query.getSelectClause());
	REQUIRE(std::get<AttrRef>(select_tuple[0]) == AttrRef(Synonym("a"), AttrName::StmtNo));
	REQUIRE(std::get<Synonym>(select_tuple[1]).getSynonym() == "v");
	std::vector<Declaration> decl_list = query.getDeclarations();
	REQUIRE(decl_list[0].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[0].getSynonym().getSynonym() == "u");
	REQUIRE(decl_list[1].getDesignEntity() == DesignEntity::VARIABLE);
	REQUIRE(decl_list[1].getSynonym().getSynonym() == "v");
	REQUIRE(decl_list[2].getDesignEntity() == DesignEntity::PROCEDURE);
	REQUIRE(decl_list[2].getSynonym().getSynonym() == "p");
	REQUIRE(decl_list[3].getDesignEntity() == DesignEntity::CONSTANT);
	REQUIRE(decl_list[3].getSynonym().getSynonym() == "c");
	REQUIRE(decl_list[4].getDesignEntity() == DesignEntity::ASSIGN);
	REQUIRE(decl_list[4].getSynonym().getSynonym() == "a");
	REQUIRE(decl_list[5].getDesignEntity() == DesignEntity::CALL);
	REQUIRE(decl_list[5].getSynonym().getSynonym() == "b");
	REQUIRE(decl_list[6].getDesignEntity() == DesignEntity::WHILE);
	REQUIRE(decl_list[6].getSynonym().getSynonym() == "w");
	std::vector<SuchThatClause> suchThatClause = query.getSuchThatClause();
	REQUIRE(suchThatClause[0] == SuchThatClause(Relationship::ModifiesP, Synonym("p"), Synonym("u"), decl_list));
	REQUIRE(suchThatClause[1] == SuchThatClause(Relationship::ModifiesS, Synonym("a"), Synonym("v"), decl_list));
	std::vector<PatternClause> patternClause = query.getPatternClause();
	REQUIRE(patternClause[0] == PatternClause(Synonym("a"), QuotedIdentifier("y"), Underscore()));
	REQUIRE(patternClause[1] == PatternClause(Synonym("w"), QuotedIdentifier("x"), Underscore()));
	std::vector<WithClause> withClause = query.getWithClause();
	REQUIRE(withClause[0] == WithClause(WithRef(AttrRef(Synonym("u"), AttrName::VarName)), WithRef(AttrRef(Synonym("b"), AttrName::ProcName))));
	REQUIRE(withClause[1] == WithClause(WithRef(AttrRef(Synonym("c"), AttrName::Value)), WithRef(AttrRef(Synonym("a"), AttrName::StmtNo))));
	REQUIRE(withClause[2] == WithClause(WithRef(AttrRef(Synonym("v"), AttrName::VarName)), WithRef(WithRef(QuotedIdentifier("x")))));
}
}


