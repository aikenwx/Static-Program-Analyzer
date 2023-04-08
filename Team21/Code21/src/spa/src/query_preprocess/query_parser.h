#pragma once

#include "query/query.h"
#include "query/query_exceptions.h"

#include <string>
#include <vector>
#include <cctype>

namespace qps {

// QueryParser represents a Query Parser consuming list of PQL String tokens to produce Query object
class QueryParser {

	private:
		std::vector<Declaration> declarations;
		std::vector<SuchThatClause> suchThatClause;
		std::vector<PatternClause> patternClause;
		std::vector<WithClause> withClause;
		Result selectClause;
		int currentIndex;
		std::vector<std::string> tokens;

		auto peek() -> std::string;
		auto next() -> std::string;
		auto isEnd() -> bool;

		static auto isTokenValidInteger(std::string str) -> bool;
		auto isSameToken(const std::string& str) -> bool;
		auto assertNextToken(const std::string& str) -> bool;

		auto parseRef() -> Ref;
		auto parseExpressionSpec() -> ExpressionSpec;
		static auto validateExpression(std::string str) -> std::string;
		static void checkAlphanumericHelper(std::string str, int &idx, int &next, int &prev);
		static void checkNumberHelper(std::string str, int &idx, int last, int &next, int &prev);
		static void checkLeftBracketHelper(std::string str, int &idx, int last, int &prev, int &next, int &openBracketCount);
		static void checkRightBracketHelper(std::string str, int &idx, int last, int &prev, int &next, int &closeBracketCount, int &openBracketCount);
		static void checkOperatorHelper(std::string str, int &idx, int last, int &next, int &prev);
		auto parseElement() -> Element;
		auto parseWithRef() -> WithRef;
		auto parseTupleSelect() -> std::vector<Element>;

		void parseSuchThat();
		void parsePattern();
		void parseWith();

		auto parseDeclaration() -> bool;
		void parseSelectClause();
		auto parseSuchThatClause() -> bool;
		auto parsePatternClause() -> bool;
		auto parseWithClause() -> bool;

		static auto operatorHelper(char character) -> int;
		static auto makePostfix(std::string str) -> std::string;

	public:
		explicit QueryParser(std::vector<std::string> tokens_);
		auto parse() -> Query;

};
}  // namespace qps
