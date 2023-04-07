#include "SyntacticValidator.h"

namespace qps {

SyntacticValidator::SyntacticValidator(Query& query) :QueryValidator(query) {}

//checks syntactically query by doing checks
auto SyntacticValidator::validateQuery() -> bool {
	checkSuchThatCorrectRefTypes();
	checkPatternCorrectRefTypes();
	return true;
}

//Check if type in relationship is corrrect
void SyntacticValidator::checkSuchThatCorrectRefTypes() {
	std::vector<SuchThatClause> such = getQuery().getSuchThatClause();
	for (auto & clau : such) {
		Ref ref1 = clau.getArg1();
		Ref ref2 = clau.getArg2();
		Relationship rel = clau.getRelationship();
		std::string relStr = getStringFromRelationship(rel);
		if (rel == Relationship::Follows || rel == Relationship::FollowsT || rel == Relationship::Parent || rel == Relationship::ParentT ||
			rel == Relationship::Next || rel == Relationship::NextT || rel == Relationship::Affects || rel == Relationship::AffectsT) {
			if (std::holds_alternative<QuotedIdentifier>(ref1) || std::holds_alternative<QuotedIdentifier>(ref2)) {
				throw  QueryException(ErrorType::Syntactic, "Syntactic error. The argument is not of correct ref type for " + relStr);
			}
		}
		else if (rel == Relationship::ModifiesS || rel == Relationship::UsesS) {
			if (std::holds_alternative<QuotedIdentifier>(ref1) || std::holds_alternative<StatementNumber>(ref2)) {
				throw  QueryException(ErrorType::Syntactic, "Syntactic error. The argument is not of correct ref type for " + relStr);
			}
		}
		else if (rel == Relationship::ModifiesP || rel == Relationship::UsesP || rel == Relationship::Calls || rel == Relationship::CallsT) {
			if (std::holds_alternative<StatementNumber>(ref1) || std::holds_alternative<StatementNumber>(ref2)) {
				throw  QueryException(ErrorType::Syntactic, "Syntactic error. The argument is not of correct ref type for " + relStr);
			}
		}
	}
}

//check if type in pattern is corrrect
void SyntacticValidator::checkPatternCorrectRefTypes() {
	std::vector<PatternClause> patt = getQuery().getPatternClause();
	auto declarations = getQuery().getDeclarations();
	for (auto & clau : patt) {
		Ref arg1 = clau.getArg1();
		ExpressionSpec arg2 = clau.getArg2();
		Synonym stmtSyn = clau.getStmtSynonym();
		auto declaration = Declaration::findDeclarationWithSynonym(declarations, stmtSyn);
		auto synDE = declaration->getDesignEntity();
		if (std::holds_alternative<StatementNumber>(arg1)) {
			throw QueryException(ErrorType::Syntactic, "Syntactic error. The first argument is not of correct ref type for assign clause");
		}
		if ((synDE == DesignEntity::WHILE || synDE == DesignEntity::IF) && std::holds_alternative<Expression>(arg2)) {
			throw QueryException(ErrorType::Syntactic, "Syntactic error. The second argument of if/while pattern cannot be expression");
		}
	}
}

}  // namespace qps
