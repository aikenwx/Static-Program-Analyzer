#pragma once

#include <iostream>

namespace qps {

class Expression {
	private:
		bool isPartial;
		std::string expression;

	public:
		Expression(bool isPartial_, std::string expression_);
		[[nodiscard]] auto isExpressionPartial() const -> bool;
		auto getExpression() -> std::string;

		auto operator==(const Expression& expr) const -> bool {
			return isPartial == expr.isPartial && expression == expr.expression;
		}

		friend auto operator<<(std::ostream& ostream, Expression& expr) -> std::ostream& {
            ostream << expr.expression;
			return ostream;
		}

};
}  // namespace qps
