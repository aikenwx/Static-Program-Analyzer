#include "SyntacticValidator.h"

namespace qps {

	SyntacticValidator::SyntacticValidator(Query query) :QueryValidator(query) {}

	bool SyntacticValidator::validateQuery() {
		checkSuchThatCorrectRefTypes();
		checkAssignPatternCorrectRefTypes();
		return true;
	}

	bool SyntacticValidator::isFactorValidInteger(std::string str) {
		for (int i = 0; i < str.length(); i++) {
			if (!std::isdigit(str[i])) {
				return false;
			}
		}
		if (str.length() > 1 && str[0] == '0') {
			return false;
		}
		return str.length() != 0;
	}

	bool SyntacticValidator::isValidFactor(std::string str) {
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		if (isFactorValidInteger(str)) {
			return true;
		} else if (std::regex_match(str, std::regex("[a-zA-Z]([a-zA-Z]|\\d)*"))) {
			return true;
		} else {
			return false;
		}

	}

	void SyntacticValidator::checkSuchThatCorrectRefTypes() {
		std::vector<SuchThatClause> such = getQuery().getSuchThatClause();
		for (int i = 0; i < such.size(); i++) {
			Ref ref1 = such[i].getArg1();
			Ref ref2 = such[i].getArg2();
			Relationship re = such[i].getRelationship();
			std::string relStr = getStringFromRelationship(re);
			if (re == Relationship::Follows || re == Relationship::FollowsT || re == Relationship::Parent || re == Relationship::ParentT) {
				if (std::holds_alternative<QuotedIdentifier>(ref1) || std::holds_alternative<QuotedIdentifier>(ref2)) {
					throw  QueryException(ErrorType::Syntactic, "Syntactic error. The argument is not of correct ref type for " + relStr);
				}
			}
			else if (re == Relationship::ModifiesS || re == Relationship::UsesS) {
				if (std::holds_alternative<QuotedIdentifier>(ref1) || std::holds_alternative<StatementNumber>(ref2)) {
					throw  QueryException(ErrorType::Syntactic, "Syntactic error. The argument is not of correct ref type for " + relStr);
				}
			}
			else if (re == Relationship::ModifiesP || re == Relationship::UsesP) {
				if (std::holds_alternative<StatementNumber>(ref1) || std::holds_alternative<StatementNumber>(ref2)) {
					throw  QueryException(ErrorType::Syntactic, "Syntactic error. The argument is not of correct ref type for " + relStr);
				}
			}
		}
	}

	//TBD expression-spec does not have parser to parse and check if valid. Need to add something to check. Now only check for first arg.
	void SyntacticValidator::checkAssignPatternCorrectRefTypes() {
		std::vector<PatternClause> patt = getQuery().getPatternClause();
		for (int i = 0; i < patt.size(); i++) {
			Ref ref3 = patt[i].getArg1();
			if (std::holds_alternative<StatementNumber>(ref3)) {
				throw  QueryException(ErrorType::Syntactic, "Syntactic error. The first argument is not of correct ref type for assign clause");
			}
			ExpressionSpec exprSpec = patt[i].getArg2();
			if (std::holds_alternative<Expression>(exprSpec)) {
				Expression expr = std::get<Expression>(exprSpec);
				if (!isValidFactor(expr.getExpression())) {
					throw  QueryException(ErrorType::Syntactic, "Syntactic error. Incorrect form for expression");
				}
			}
		}
	}
}
