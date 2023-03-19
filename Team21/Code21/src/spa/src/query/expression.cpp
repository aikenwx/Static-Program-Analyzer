#include "query/expression.h"

#include <utility>





namespace qps {

	Expression::Expression(bool isPartial_, std::string expression_)
		: isPartial{ isPartial_ }, expression{std::move(std::move(std::move(std::move( std::move(expression_) ))))} {}

	auto Expression::isExpressionPartial() const -> bool {
		return isPartial;
	}


	auto Expression::getExpression() -> std::string {
		return expression;
	}
}  // namespace qps
