#include "query_preprocess/query_parser.h"

namespace qps {

	QueryParser::QueryParser(std::vector<std::string> tokens_)
		: tokens{ tokens_ }, currentIndex{ 0 } {}

	std::string QueryParser::peek() {
		if (currentIndex < tokens.size()) {
			return tokens[currentIndex];
		}
		return "";
	}

	std::string QueryParser::next() {
		std::string currentToken{ peek() };
		currentIndex++;
		return currentToken;
	}
	bool QueryParser::isEnd() {
		return (peek() == "");
	}

	bool QueryParser::isTokenAllDigits(std::string str) {
		for (int i = 0; i < str.length(); i++) {
			if (!std::isdigit(str[i])) {
				return false;
			}
		}
		return str.length() != 0;
	}

	bool QueryParser::isSameToken(std::string str) {
		return (peek() == str);
	}

	bool QueryParser::assertNextToken(std::string str) {
		if (isSameToken(str)) {
			return true;
		}
		throw QueryException("Invalid Query Syntax. Expect (" + str + ") got (" + peek() + ")");
	}

	Ref QueryParser::parseRef() {
		if (isSameToken("\"")) {
			next();
			std::string id = next();
			assertNextToken("\"");
			next();
			return QuotedIdentifier(id);
		} else if (Synonym::isValidSynonym(peek())) {
			return Synonym(next());
		} else if (isSameToken("_")) {
			next();
			return Underscore();
		} else if (isTokenAllDigits(peek())) {
			return std::stoi(next());
		} else {
			throw QueryException("Invalid representation for Ref: (" + peek() + ")");
		}
	}

	ExpressionSpec QueryParser::parseExpression() {
		bool isPartial{};
		std::string expression{};

		if (isSameToken("\"")) {
			next();
			expression = next();
			isPartial = false;
			assertNextToken("\"");
			next();
		} else {
			assertNextToken("_");
			next();
			if (isSameToken("\"")) {
				next();
				expression = next();
				isPartial = true;
				assertNextToken("\"");
				next();
				assertNextToken("_");
				next();
			} else {
				return Underscore();
			}
		}
		return Expression(isPartial, expression);
	}

	bool QueryParser::parseDeclaration() {
		try {
			getDesignEntityFromString(peek());
		} catch (QueryException& e) {
			return false;
		}
		DesignEntity design_entity{ getDesignEntityFromString(next()) };
		std::vector<Synonym> synonym_list;

		synonym_list.push_back(Synonym(next()));

		while (!isSameToken(";")) {
			assertNextToken(",");
			next();
			synonym_list.push_back(Synonym(next()));
		}
		next();
		for (Synonym synonym : synonym_list) {
			declarations.push_back(Declaration(design_entity, synonym));
		}
		return true;
	}
	void QueryParser::parseSelectClause() {
		assertNextToken("Select");
		next();
		Synonym test_synonym{ peek() };
		if (Declaration::findDeclarationWithSynonym(declarations, test_synonym) == std::nullopt) {
			throw QueryException("Semantic error. There is missing declaration in Select clause for " + peek());
		}
		Synonym synonym{ next() };
		DesignEntity design_entity = Declaration::findDeclarationWithSynonym(declarations, synonym)->getDesignEntity();
		selectClause.push_back(Declaration(design_entity, synonym));
	}

	bool QueryParser::parseSuchThatClause() {
		if (!isSameToken("such")) {
			return false;
		}
		next();
		assertNextToken("that");
		next();

		Relationship relationship{ getRelationshipFromString(next()) };
		assertNextToken("(");
		next();
		Ref arg1{ parseRef() };
		assertNextToken(",");
		next();
		Ref arg2{ parseRef() };
		assertNextToken(")");
		next();

		suchThatClause.push_back(SuchThatClause(relationship, arg1, arg2, declarations));
		return true;
	}

	bool QueryParser::parsePatternClause() {
		if (!isSameToken("pattern")) {
			return false;
		}

		next();
		std::string syn{ next() };
		Synonym synonym{ Synonym(syn) };
		auto declaration{ Declaration::findDeclarationWithSynonym(declarations, synonym) };
		if (declaration->getDesignEntity() != DesignEntity::ASSIGN) {
			throw QueryException("Invalid syntax for pattern assign with synonym: " + syn);
		}

		assertNextToken("(");
		next();
		Ref arg1{ parseRef() };
		assertNextToken(",");
		next();
		ExpressionSpec arg2{ parseExpression() };
		assertNextToken(")");
		next();

		patternClause.push_back(PatternClause(synonym, arg1, arg2));
		return true;
	}

	Query QueryParser::parse() {
		while (!isEnd()) {
			if (!parseDeclaration()) {
				break;
			}
		}
		parseSelectClause();
		while (!isEnd()) {
			if (parseSuchThatClause()) {
				continue;
			}
			else if (parsePatternClause()) {
				continue;
			}
			else {
				throw QueryException("Invalid clause, not such-that or pattern");
			}
		}
		return Query(declarations, suchThatClause, patternClause, selectClause[0]);
	}
}
