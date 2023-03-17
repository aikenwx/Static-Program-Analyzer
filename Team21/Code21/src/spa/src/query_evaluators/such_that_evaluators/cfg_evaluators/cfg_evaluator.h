#pragma once

#include <queue>
#include <unordered_set>

#include "query/declaration.h"
#include "query/such_that_clause.h"
#include "query_evaluators/such_that_evaluators/such_that_evaluator.h"

namespace qps {

class Cfg_Evaluator : public SuchThatEvaluator {
 public:
  Cfg_Evaluator(SuchThatClause clause, std::vector<Declaration> declarations) :
      SuchThatEvaluator(std::move(clause), std::move(declarations)) {}

};

auto GetStmtNo(Entity *entity) -> int;
auto GetEntity(int stmt_no, QueryFacade &pkb) -> Entity *;
auto GetEntities(Synonym &syn, std::vector<Declaration> &declarations, QueryFacade &pkb) -> std::vector<Entity *> *;
auto MatchesEntityType(Entity *entity, EntityType type) -> bool;
auto Next(cfg::CFG &cfg, int stmt_no) -> std::vector<int>;
auto Next(cfg::CFG &cfg, Entity *entity) -> std::vector<int>;
auto Prev(cfg::CFG &cfg, int stmt_no) -> std::vector<int>;
auto Prev(cfg::CFG &cfg, Entity *entity) -> std::vector<int>;

template<typename T>
auto HasNext(cfg::CFG &cfg, T stmt) -> bool {
  return !Next(cfg, stmt).empty();
}

template<typename T>
auto HasPrev(cfg::CFG &cfg, T stmt) -> bool {
  return !Prev(cfg, stmt).empty();
}

template<typename Condition>
auto FindEntitiesMatchingCondition(QueryFacade &pkb, EntityType type) -> std::vector<Entity *> {
  auto *entities = pkb.getEntitiesByType(type);
  std::vector<Entity *> filtered_entities;
  std::copy_if(entities->begin(), entities->end(), std::back_inserter(filtered_entities),
               [&](Entity *entity) { return Condition(type); });
  return filtered_entities;
}

template<typename SrcPredicate, typename DestPredicate, typename NeighborFunc, typename RowAction>
void FindReachableEntities(const std::vector<Entity *> &sources,
                           NeighborFunc get_nei,
                           SrcPredicate include_src,
                           DestPredicate include_dest,
                           RowAction row_action,
                           QueryFacade &pkb) {
  auto *cfg = pkb.getCFG();
  std::unordered_set<int> seen;
  for (const auto &source : sources) {
    int src_stmt_no = GetStmtNo(source);
    if (seen.find(src_stmt_no) != seen.end()) {
      continue;
    }
    auto opt_start = cfg->GetBlockAt(src_stmt_no);
    if (!opt_start) {
      continue;
    }
    std::unordered_set<Entity *> active_src{source};
    auto start_block = opt_start.value();
    std::unordered_map<int, int> block_seen = {{start_block->start(), 1}};
    std::queue<std::shared_ptr<cfg::Block>> blocks;
    blocks.push(start_block);
    while (!blocks.empty()) {
      auto curr_block = blocks.front();
      blocks.pop();
      auto start = curr_block->end();
      if (curr_block->IsInBlock(src_stmt_no) && block_seen[curr_block->start()] == 1) {
        start = src_stmt_no - 1;
      }
      for (int stmt = start; stmt >= curr_block->start(); --stmt) {
        Entity *stmt_entity = GetEntity(stmt, pkb);
        if (include_dest(stmt_entity)) {
          for (auto *src : active_src) {
            row_action(src, stmt_entity);
          }
        }
        if (include_src(stmt_entity)) {
          active_src.insert(stmt_entity);
          seen.insert(stmt);
        }
      }
      for (auto &nei : get_nei(curr_block)) {
        if (block_seen[nei->start()] == 2) {
          continue;
        }
        block_seen[nei->start()] += 1;
        blocks.push(nei);
      }
    }
  }
}

const auto BlockChildren = [](std::shared_ptr<cfg::Block> &block) {
  return block->children();
};

const auto BlockParents = [](std::shared_ptr<cfg::Block> &block) {
  return block->parents();
};

auto Reachable(int src, int dest, QueryFacade &pkb) -> bool;

template<typename NeighborFunc>
auto FindReachableEntities(int src, EntityType dest, NeighborFunc get_nei, QueryFacade &pkb) -> std::vector<Entity *> {
  auto *entity = GetEntity(src, pkb);
  std::vector<Entity *> dest_entities;
  FindReachableEntities({entity},
                        get_nei,
                        [src](Entity *entity) { return GetStmtNo(entity) == src; },
                        [&](Entity *entity) { return MatchesEntityType(entity, dest); },
                        [&](Entity *src, Entity *dst) { dest_entities.push_back(dst); },
                        pkb);
  return dest_entities;
}
} // qps
