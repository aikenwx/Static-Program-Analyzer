#include "next_evaluator.h"

#include "query/ref.h"

namespace qps {
class NextVisitor {
 public:
  NextVisitor(QueryFacade &pkb, std::vector<Declaration> &declarations)
      : pkb_(pkb), cfg_(*pkb.getCFG()), declarations_(declarations) {}

  auto operator()(StatementNumber from, StatementNumber dest) -> ClauseResult {
    auto next_statements = DirectNeighbors<ForwardBlockIterator>(cfg_, from);
    return find(next_statements.begin(), next_statements.end(), dest) != next_statements.end();
  }

  auto operator()(StatementNumber from, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    return HasNext(cfg_, from);
  }

  auto operator()([[maybe_unused]] Underscore underscore, StatementNumber dest) -> ClauseResult {
    return HasPrev(cfg_, dest);
  }

  auto operator()(StatementNumber src, Synonym dest) -> ClauseResult {
    auto next_statements = DirectNeighbors<ForwardBlockIterator>(cfg_, src);
    auto filtered_entities = FilterEntities(next_statements, GetEntityType(dest, declarations_));
    return SynonymTable{dest, filtered_entities};
  }

  auto operator()(Synonym from, StatementNumber dest) -> ClauseResult {
    auto prev_statements = DirectNeighbors<ReverseBlockIterator>(cfg_, dest);
    auto filtered_entities = FilterEntities(prev_statements, GetEntityType(from, declarations_));
    return SynonymTable{from, filtered_entities};
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
    if(src == dest) return false;
    auto src_entity_type = GetEntityType(src, declarations_);
    const auto *src_entities = pkb_.getEntitiesByType(src_entity_type);
    auto dest_entity_type = GetEntityType(dest, declarations_);
    const auto *dest_entities = pkb_.getEntitiesByType(dest_entity_type);
    if (src_entities->size() < dest_entities->size()) {
      auto rows = GetRelationships<ForwardBlockIterator>(*src_entities, dest_entity_type);
      return SynonymTable{{src, dest}, rows};
    }
    auto rows = GetRelationships<ReverseBlockIterator>(*dest_entities, src_entity_type);
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

  auto FilterEntities(const std::vector<int> &statements, EntityType filter) -> std::vector<Entity *> {
    std::vector<Entity *> entities;
    for (auto stmt : statements) {
      auto *entity = pkb_.getEntity(filter, stmt);
      if (entity == nullptr) {
        continue;
      }
      entities.push_back({entity});
    }
    return entities;
  }

  template<typename BlockIterator>
  auto GetRelationships(const std::vector<Entity *> &entities,
                        EntityType filter) -> std::vector<std::vector<Entity *>> {
    std::vector<SynonymTable::Row> rows;
    for (const auto &left : entities) {
      auto next_statements = DirectNeighbors<BlockIterator>(cfg_, left);
      auto filtered_entities = FilterEntities(next_statements, filter);
      for (const auto &right : filtered_entities) {
        rows.push_back({left, right});
      }
    }
    return rows;
  }
};

auto NextEvaluator::Evaluate(QueryFacade &pkb) -> ClauseResult {
  return std::visit(NextVisitor{pkb, declarations_}, clause_.getArg1(), clause_.getArg2());
}
}  // namespace qps
