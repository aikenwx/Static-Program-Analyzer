#pragma once

#include "query/query.h"
#include "query/query_exceptions.h"

#include <string>
#include <vector>
#include <cctype>

namespace qps {

class QueryParser {
	private:
		std::vector<Declaration> declarations;
		std::vector<SuchThatClause> suchThatClause;
		std::vector<PatternClause> patternClause;
		std::vector<Declaration> selectClause;
		int currentIndex;
		std::vector<std::string> tokens;

		std::string peek();
		std::string prev();
		std::string next();
		bool isEnd();

		bool isTokenValidInteger(std::string str);
		bool isSameToken(std::string str);
		bool assertNextToken(std::string str);

		Ref parseRef();
		ExpressionSpec parseExpression();

		bool parseDeclaration();
		bool parseSuchThatClause();
		bool parsePatternClause();
		void parseSelectClause();

	public:
		QueryParser(std::vector<std::string> tokens_);
		Query parse();
};
}

