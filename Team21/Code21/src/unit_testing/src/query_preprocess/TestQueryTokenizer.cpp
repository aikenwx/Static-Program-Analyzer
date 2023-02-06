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
