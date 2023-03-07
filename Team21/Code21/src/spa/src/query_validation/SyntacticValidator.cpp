#include "SyntacticValidator.h"

namespace qps {

	SyntacticValidator::SyntacticValidator(Query query) :QueryValidator(query) {}

	bool SyntacticValidator::validateQuery() {
		checkSuchThatCorrectRefTypes();
		checkAssignPatternCorrectRefTypes();
		return true;
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
			else if (re == Relationship::ModifiesP || re == Relationship::UsesP || re == Relationship::Calls || re == Relationship::CallsT) {
				if (std::holds_alternative<StatementNumber>(ref1) || std::holds_alternative<StatementNumber>(ref2)) {
					throw  QueryException(ErrorType::Syntactic, "Syntactic error. The argument is not of correct ref type for " + relStr);
				}
			}
		}
	}

	//expression-spec is checked when the expression is converted to postfix in the evaluator. Now only check for first arg.
	void SyntacticValidator::checkAssignPatternCorrectRefTypes() {
		std::vector<PatternClause> patt = getQuery().getPatternClause();
		for (int i = 0; i < patt.size(); i++) {
			Ref ref3 = patt[i].getArg1();
			if (std::holds_alternative<StatementNumber>(ref3)) {
				throw  QueryException(ErrorType::Syntactic, "Syntactic error. The first argument is not of correct ref type for assign clause");
			}
		}
	}
}
