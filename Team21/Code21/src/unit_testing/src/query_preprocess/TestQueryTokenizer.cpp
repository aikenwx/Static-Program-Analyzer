#include <utility>

#include "catch.hpp"
#include "query_preprocess/query_tokenizer.h"
#include "query_preprocess/query_parser.h"

TEST_CASE("Tokenizer: One Declaration, one synonym") {
	qps::QueryTokenizer tokenizer("variable v; Select v");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "v", ";", "Select", "v" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: One Declarations, two synonyms") {
	qps::QueryTokenizer tokenizer("variable u, v; Select v");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ",", "v", ";", "Select", "v" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: Two Declarations, two synonyms") {
	qps::QueryTokenizer tokenizer("variable u; procedure p; Select p");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ";", "procedure", "p", ";", "Select", "p" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: Two Declarations, four synonyms") {
	qps::QueryTokenizer tokenizer("variable u, v; procedure p, q; Select q");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ",", "q", ";", "Select", "q" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: pattern(syn,expr)") {
	qps::QueryTokenizer tokenizer("variable u, v; assign a; Select a pattern a (u, \"x + y\")");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "u", ",", "\"", "x + y", "\"",")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: pattern(_,partial_expr)") {
	qps::QueryTokenizer tokenizer("variable u, v; assign a; Select a pattern a (u, _\"x + y\"_)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "u", ",", "_", "\"", "x + y", "\"", "_", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: pattern(quoted_id,_)") {
	qps::QueryTokenizer tokenizer("variable u, v; assign a; Select a pattern a (\"y\",_)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ",", "v", ";", "assign", "a", ";", "Select", "a", "pattern", "a", "(", "\"", "y", "\"", ",", "_", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: follows(syn,syn)") {
	qps::QueryTokenizer tokenizer("assign a, b; Select a such that Follows (a,b)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Follows", "(", "a", ",", "b", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: follows*(_,_)") {
	qps::QueryTokenizer tokenizer("assign a, b; Select a such that Follows* (_,_)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Follows*", "(", "_", ",", "_", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: parent(syn,stmtNo)") {
	qps::QueryTokenizer tokenizer("assign a, b; Select a such that Parent (a,2)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Parent", "(", "a", ",", "2", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: parentT(stmtNo,stmtNo)") {
	qps::QueryTokenizer tokenizer("assign a, b; Select a such that Parent* (1,2)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Parent*", "(", "1", ",", "2", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: modifiesS(stmtNo,syn)") {
	qps::QueryTokenizer tokenizer("variable u; assign a, b; Select a such that Modifies (1,u)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ";", "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Modifies", "(", "1", ",", "u", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: modifiesP(syn,quoted_id)") {
	qps::QueryTokenizer tokenizer("variable u; procedure a, b; Select a such that Modifies (a,\"x\")");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ";", "procedure", "a", ",", "b", ";", "Select", "a", "such", "that", "Modifies", "(", "a", ",", "\"", "x", "\"", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: usesS(syn,_)") {
	qps::QueryTokenizer tokenizer("variable u; assign a, b; Select a such that Uses (a,_)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ";", "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Uses", "(", "a", ",", "_", ")"};
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: usesP(quoted_id,_)") {
	qps::QueryTokenizer tokenizer("variable u; assign a, b; Select a such that Uses (\"x\",_)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ";", "assign", "a", ",", "b", ";", "Select", "a", "such", "that", "Uses", "(", "\"", "x", "\"", ",", "_", ")" };
	REQUIRE(tokens == correct_tokens);
}

TEST_CASE("Tokenizer: pattern + such that") {
	qps::QueryTokenizer tokenizer("variable u, v; procedure p; assign a; Select p such that Modifies(p,u) pattern a (\"y\",_)");
	std::vector<std::string> tokens = tokenizer.tokenize();
	std::vector<std::string> correct_tokens{ "variable", "u", ",", "v", ";", "procedure", "p", ";", "assign", "a", ";", "Select",
		"p", "such", "that", "Modifies", "(", "p", ",", "u", ")", "pattern", "a", "(", "\"", "y", "\"", ",", "_", ")" };
	REQUIRE(tokens == correct_tokens);
}