#pragma once

#include "query/declaration.h"
#include "query/pattern_clause.h"
#include "query_evaluators/clause_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

namespace qps {

	class PatternEvaluator : public ClauseEvaluator {
	public:

		PatternEvaluator(PatternClause clause, std::vector<Declaration> declarations);

		ClauseResult Evaluate(QueryFacade& pkb) override;

		virtual std::vector<EntityType> CallPkb(QueryFacade& pkb) = 0;

	private:

		ClauseResult ConstructResult(const std::vector<EntityType>&);
		PatternClause clause_;
		std::vector<Declaration> declarations_;

	};

} // qps
