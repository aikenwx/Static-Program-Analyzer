#include <utility>

#include "query/pattern_clause.h"

namespace qps {

	PatternClause::PatternClause(Synonym synonym_, Ref arg1_, ExpressionSpec arg2_)
		: stmtSynonym{std::move( synonym_ )}, arg1{std::move( arg1_ )}, arg2{std::move( std::move(arg2_) )}, gotExpression(std::holds_alternative<Expression>(arg2)), isPartial{ false } {

		if (std::holds_alternative<Underscore>(arg2)) {
			isPartial = false;
		} else {
			auto *arg2_expression = std::get_if<Expression>(&arg2);
			isPartial = arg2_expression->isExpressionPartial();
		}
	}
[[maybe_unused]] auto PatternClause::clauseGotExpression() const -> bool {
		return gotExpression;
	}
[[maybe_unused]] auto PatternClause::isExpressionPartial() const -> bool {
		return isPartial;
	}
	auto PatternClause::getArg1() -> Ref {
		return arg1;
	}
	auto PatternClause::getArg2() -> ExpressionSpec {
		return arg2;
	}
	auto PatternClause::getStmtSynonym() -> Synonym {
		return stmtSynonym;
	}
}  // namespace qps
