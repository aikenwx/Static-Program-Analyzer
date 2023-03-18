#include "next_star_evaluator.h"
#include "query/ref.h"

namespace qps {
class NextStarVisitor {
 public:
  NextStarVisitor(QueryFacade &pkb, std::vector<Declaration> &declarations)
      : pkb_(pkb), cfg_(*pkb.getCFG()), declarations_(declarations) {}

  auto operator()(StatementNumber from, StatementNumber dest) -> ClauseResult {
    return Reachable<ForwardBlockIterator>(from, dest, pkb_);
  }

  auto operator()(StatementNumber from, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    return HasNext(cfg_, from);
  }

  auto operator()([[maybe_unused]] Underscore underscore, StatementNumber dest) -> ClauseResult {
    return HasPrev(cfg_, dest);
  }

  auto operator()(StatementNumber src, Synonym dest) -> ClauseResult {
    auto dest_entities = FindReachableEntities<ForwardBlockIterator>(src,
                                                                     GetEntityType(dest, declarations_),
                                                                     pkb_);
    return SynonymTable{std::move(dest), dest_entities};
  }

  auto operator()(Synonym from, StatementNumber dest) -> ClauseResult {
    auto src_entities = FindReachableEntities<ReverseBlockIterator>(dest,
                                                                    GetEntityType(from, declarations_),
                                                                    pkb_);
    return SynonymTable{std::move(from), src_entities};
  }

  auto operator()(Synonym src, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    auto *entities = GetEntities(src, declarations_, pkb_);
    std::vector<Entity *> filtered_entities;
    std::copy_if(entities->begin(), entities->end(), std::back_inserter(filtered_entities),
                 [&](Entity *entity) { return HasNext(cfg_, entity); });
    return SynonymTable{std::move(src), filtered_entities};
  }

  auto operator()([[maybe_unused]] Underscore underscore, Synonym dest) -> ClauseResult {
    auto *entities = GetEntities(dest, declarations_, pkb_);
    std::vector<Entity *> filtered_entities;
    std::copy_if(entities->begin(), entities->end(), std::back_inserter(filtered_entities),
                 [&](Entity *entity) { return HasPrev(cfg_, entity); });
    return SynonymTable{std::move(dest), filtered_entities};
  }

  auto operator()(Synonym src, Synonym dest) -> ClauseResult {
    auto src_entity_type = GetEntityType(src, declarations_);
    auto *src_entities = pkb_.getEntitiesByType(src_entity_type);
    auto dest_entity_type = GetEntityType(dest, declarations_);
    auto *dest_entities = pkb_.getEntitiesByType(dest_entity_type);
    if (src_entities->size() < dest_entities->size()) {
      auto rows = FindReachableEntities<ForwardBlockIterator>(*src_entities,
                                                              src_entity_type,
                                                              dest_entity_type,
                                                              pkb_);
      return SynonymTable{{src, dest}, std::move(rows)};
    }
    auto rows = FindReachableEntities<ReverseBlockIterator>(*dest_entities,
                                                            dest_entity_type,
                                                            src_entity_type,
                                                            pkb_);
    return SynonymTable{{dest, src}, std::move(rows)};

  }

  auto operator()([[maybe_unused]] Underscore underscore_1, [[maybe_unused]] Underscore underscore_2) -> ClauseResult {
    auto *entities = pkb_.getEntitiesByType(Statement::getEntityTypeStatic());
    return std::any_of(entities->begin(), entities->end(),
                       [&](Entity *entity) { return HasNext(cfg_, entity); });
  }

  template<typename Left, typename Right>
  auto operator()([[maybe_unused]] Left left, [[maybe_unused]] Right right) -> ClauseResult {
    throw std::invalid_argument("Invalid clause for Next");
  }

 private:
  QueryFacade &pkb_;
  cfg::CFG &cfg_;
  std::vector<Declaration> &declarations_;
  std::vector<Synonym> syns_;
};

auto NextStarEvaluator::Evaluate(QueryFacade &pkb) -> ClauseResult {
  return std::visit(NextStarVisitor{pkb, declarations_}, clause_.getArg1(), clause_.getArg2());
}

}  // namespace qps
