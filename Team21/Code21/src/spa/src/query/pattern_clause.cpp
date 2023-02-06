#include "query/pattern_clause.h"

namespace qps {

	PatternClause::PatternClause(Synonym synonym, Ref arg1_, ExpressionSpec arg2_)
		: assignSynonym{ synonym }, arg1{ arg1_ }, arg2{ arg2_ }, isPartial{ false } {
		gotExpression = std::holds_alternative<Expression>(arg2);
		if (std::holds_alternative<Underscore>(arg2)) {
			isPartial = false;
		} else {
			auto arg2_expression = std::get_if<Expression>(&arg2);
			isPartial = arg2_expression->isExpressionPartial();
		}
	}
	bool PatternClause::clauseGotExpression() {
		return gotExpression;
	}
	bool PatternClause::isExpressionPartial() {
		return isPartial;
	}
	Ref PatternClause::getArg1() {
		return arg1;
	}
	ExpressionSpec PatternClause::getArg2() {
		return arg2;
	}
	Synonym PatternClause::getAssign() {
		return assignSynonym;
	}
}
