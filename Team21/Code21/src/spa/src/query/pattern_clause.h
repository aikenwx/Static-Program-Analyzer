#pragma once

#include "query/ref.h"
#include "query/expression.h"
#include "query/synonym.h"
#include "query/underscore.h"

#include <optional>
#include <variant>

namespace qps {

using ExpressionSpec = std::variant<Underscore, Expression>;

class PatternClause {
	private:
		Synonym assignSynonym;
		Ref arg1;
		ExpressionSpec arg2;
		bool gotExpression;
		bool isPartial;

	public:
		PatternClause(Synonym synonym, Ref arg1_, ExpressionSpec arg2);
		bool clauseGotExpression();
		bool isExpressionPartial();
		Ref getArg1();
		ExpressionSpec getArg2();

		bool operator==(const PatternClause& clause) const {
			return assignSynonym == clause.assignSynonym && arg1 == clause.arg1
				&& arg2 == clause.arg2 && gotExpression == clause.gotExpression
				&& isPartial == clause.isPartial;
		}

		friend std::ostream& operator<<(std::ostream& os, PatternClause& clause) {
			os << "Pattern " << clause.assignSynonym.getSynonym();
			return os;
		}
};

}

