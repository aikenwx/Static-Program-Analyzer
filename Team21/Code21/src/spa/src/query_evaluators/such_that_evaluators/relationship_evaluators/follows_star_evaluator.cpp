#include "follows_star_evaluator.h"

namespace qps {
	std::vector<::Relationship*> FollowsStarEvaluator::CallPkb(QueryFacade& pkb, EntityType left, EntityType right) {
		std::vector<::Relationship*> res;
		auto pkb_res = pkb.getFollowsStarRelationshipsByLeftAndRightEntityTypes(left, right);
		res.assign(pkb_res->begin(), pkb_res->end());
		return res;
	}

	std::vector<EntityType> FollowsStarEvaluator::GetLeftHandTypes(Ref& left_arg) {
		return { Statement::getEntityTypeStatic() };
	}

	std::vector<EntityType> FollowsStarEvaluator::GetRightHandTypes(Ref& right_arg) {
		return { Statement::getEntityTypeStatic() };
	}
} // qps
