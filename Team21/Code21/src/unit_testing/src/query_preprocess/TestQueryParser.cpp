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
