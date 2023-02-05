/* need to include all the depedencies from evaluator clauses.Below are pseudo for now.
May have to change how entire thing works depending on how ClauseEvaluatorFactory works. Does it create special evaluators just
from looking at query object? So like it should create type per situation based on query and its clauses? So it calls constraintsSolver?*/ 
#include <iostream>

#include "query/Clause/ClauseEvaluatorFactory.h"
#include "query/Clause/selectEvaluator.h"
#include "query/Clause/SuchThatEvaluator.h"
#include "query/Clause/PatternEvaluator.h"

#include "ConstraintsSolver.h"
#include "QueryEvaluator.h"

namespace qps {
	QueryEvaluator::QueryEvaluator(Query query_) :query_(query_) {}

	void QueryEvaluator::evaluateQuery() {
		/* Should we change return type for somes of the fields to have optional or vector array of clauses for a type of clause.
		e.g. vector array containing only multiple suchThatClauses objects*/
		if (query_.getPatternClause() == NULL && query_.getSuchThatClause == NULL) {
			// Below is assumption on how the function works
			ClauseResult c = selectClauseResult();
			std::cout << c.results;
		}
		else if (query_.getPatternClause() == NULL) {
			ClauseResult c = suchThatClauseResult();
			std::cout << c.results;
		}
		else if (query_.getSuchThatClause() == NULL) {
			ClauseResult c = assignPatternClauseResult();
			std::cout << c.results;
		}
		// to be done. Not sure on constraintsSolver
		/*else {
			ClauseResult c = ConstraintsSolver::solve(query_);
			std::cout << c.results;
		} */

	}

	ClauseResult QueryEvaluator::selectClauseResult() {
		//Assumption on how it works. It creates an evalutor for select and then outputs to cout the results.
		SelectEvaluator e = ClauseEvaluatorFactory::createEvaluators(query_, "select");
		return e.evaluate();
	}

	ClauseResult QueryEvaluator::suchThatClauseResult() {
		SuchThatEvaluator e = ClauseEvaluatorFactory::createEvaluators(query_, "SuchThat");
		return e.evaluate();
	}

	ClauseResult QueryEvaluator::assignPatternClauseResult() {
		PatternEvaluator e = ClauseEvaluatorFactory::createEvaluators(query_, "Pattern");
		return e.evaluate();
	}
}