// need to include all the depedencies from evaluator clauses. Below are pseudo for now.
#include <iostream>

#include "query/Clause/ClauseEvaluatorFactory.h"
#include "query/Clause/selectEvaluator.h"
#include "query/Clause/SuchThatEvaluator.h"
#include "query/Clause/PatternEvaluator.h"

#include "ConstraintsSolver.h"

// to be added. Not very sure how to implement this.
namespace qps {
	
	ClauseResult ConstraintsSolver::solve(const Query& query) {
		
	}

	ClauseResult ConstraintsSolver::suchThatAssignPatternClauseResult(const Query& query) {

	}
}
