#pragma once

#include <iostream>

namespace qps {

class Expression {
	private:
		bool isPartial;
		std::string expression;

	public:
		Expression(bool isPartial_, std::string expression_);
		bool isExpressionPartial();
		std::string getExpression();

		bool operator==(const Expression& expr) const {
			return isPartial == expr.isPartial && expression == expr.expression;
		}

		friend std::ostream& operator<<(std::ostream& os, Expression& expr) {
			os << expr.expression;
			return os;
		}

};
}
