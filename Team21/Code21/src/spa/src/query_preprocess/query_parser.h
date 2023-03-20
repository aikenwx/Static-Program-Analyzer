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
		static auto validateExpressionHelper(std::string str) -> std::string;
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
