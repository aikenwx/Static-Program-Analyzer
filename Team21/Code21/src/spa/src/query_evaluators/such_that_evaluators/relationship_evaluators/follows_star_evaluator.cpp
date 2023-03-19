#include "follows_star_evaluator.h"

namespace qps {
	auto FollowsStarEvaluator::CallPkb(QueryFacade& pkb, EntityType left, EntityType right) -> std::vector<::Relationship*> {
		std::vector<::Relationship*> res;
		auto *pkb_res = pkb.getFollowsStarRelationshipsByLeftAndRightEntityTypes(left, right);
		res.assign(pkb_res->begin(), pkb_res->end());
		return res;
	}

	auto FollowsStarEvaluator::GetLeftHandTypes(Ref& left_arg) -> std::vector<EntityType> {
		return { Statement::getEntityTypeStatic() };
	}

	auto FollowsStarEvaluator::GetRightHandTypes(Ref& right_arg) -> std::vector<EntityType> {
		return { Statement::getEntityTypeStatic() };
	}
}  // namespace qps
