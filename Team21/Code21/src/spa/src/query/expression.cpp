#include "query/expression.h"

namespace qps {

	Expression::Expression(bool isPartial_, std::string expression_) 
		: isPartial{ isPartial_ }, expression{ expression_ } {}
	
	bool Expression::isExpressionPartial() {
		return isPartial;
	}


	std::string Expression::getExpression() {
		return expression;
	}
}
