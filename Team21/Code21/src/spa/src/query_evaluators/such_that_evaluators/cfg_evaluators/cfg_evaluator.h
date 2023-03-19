#pragma once

#include <queue>
#include <unordered_set>
#include <functional>

#include "query/declaration.h"
#include "query/such_that_clause.h"
#include "query_evaluators/such_that_evaluators/such_that_evaluator.h"
#include "query_evaluators/pkb_helpers.h"

namespace qps {

class Cfg_Evaluator : public SuchThatEvaluator {
 public:
  Cfg_Evaluator(SuchThatClause clause, std::vector<Declaration> declarations) :
      SuchThatEvaluator(std::move(clause), std::move(declarations)) {}

};

template<typename T>
auto ConvertToSharedPtrs(std::vector<std::weak_ptr<T>> weak_ptrs) -> std::vector<std::shared_ptr<T>> {
  std::vector<std::shared_ptr<cfg::Block>> shared_ptrs(weak_ptrs.begin(), weak_ptrs.end());
  return shared_ptrs;
}

struct ForwardBlockIterator {
  static auto NextStmt(int stmt_no) -> int {
    return stmt_no + 1;
  }

  static auto GetNei(std::shared_ptr<cfg::Block> &block) -> std::vector<std::shared_ptr<cfg::Block>> {
    return ConvertToSharedPtrs(block->children());
  }

  static auto BlockStart(std::shared_ptr<cfg::Block> &block) -> int {
    return block->start();
  }

  static auto BlockEnd(std::shared_ptr<cfg::Block> &block) -> int {
    return block->end();
  }
};

struct ReverseBlockIterator {
  static auto NextStmt(int stmt_no) -> int {
    return stmt_no - 1;
  }

  static auto GetNei(std::shared_ptr<cfg::Block> &block) -> std::vector<std::shared_ptr<cfg::Block>> {
    return ConvertToSharedPtrs(block->parents());
  }

  static auto BlockStart(std::shared_ptr<cfg::Block> &block) -> int {
    return block->end();
  }

  static auto BlockEnd(std::shared_ptr<cfg::Block> &block) -> int {
    return block->start();
  }
};

template<typename BlockIterator>
auto DirectNeighbors(cfg::CFG &cfg, int stmt_no) -> std::vector<int> {
  auto opt_block = cfg.GetBlockAt(stmt_no);
  if (!opt_block) {
    return {};
  }
  auto &block = opt_block.value();
  std::vector<int> neighbors;
  int next_possible_statment = BlockIterator::NextStmt(stmt_no);
  if (block->IsInBlock(next_possible_statment)) {
    neighbors.push_back(next_possible_statment);
  } else {
    for (auto &nei : BlockIterator::GetNei(block)) {
      neighbors.push_back(BlockIterator::BlockStart(nei));
    }
  }
  return neighbors;
}

template<typename BlockIterator>
auto DirectNeighbors(cfg::CFG &cfg, Entity *entity) -> std::vector<int> {
  auto *statement = dynamic_cast<Statement *>(entity);
  return DirectNeighbors<BlockIterator>(cfg, statement->getStatementNumber());
}

template<typename T>
auto HasNext(cfg::CFG &cfg, T stmt) -> bool {
  return !DirectNeighbors<ForwardBlockIterator>(cfg, stmt).empty();
}

template<typename T>
auto HasPrev(cfg::CFG &cfg, T stmt) -> bool {
  return !DirectNeighbors<ReverseBlockIterator>(cfg, stmt).empty();
}

template<typename DestPredicate, typename BlockIterator, typename ProcessRow>
class ReachableEntityFinder {
 public:
  ReachableEntityFinder(const std::vector<Entity *> &sources, BlockIterator iterator,
                        DestPredicate include_dest, ProcessRow process_row, QueryFacade &pkb)
      : sources_(sources), iterator_(iterator), include_dest_(include_dest), process_row_(process_row), pkb_(pkb) {}

  void operator()() {
    auto &cfg = *pkb_.getCFG();
    for (const auto &source : sources_) {
      ReachableEntities(source, cfg);
    }
  }

 private:
  void ReachableEntities(Entity *entity, cfg::CFG &cfg) {
    int src = GetStmtNo(entity);
    auto opt_block = cfg.GetBlockAt(src);
    if (!opt_block) {
      return;
    }
    auto start_block = opt_block.value();
    std::queue<std::shared_ptr<cfg::Block>> bfs_frontier;
    std::unordered_set<int> blocks_seen;

    for (auto nei : iterator_.GetNei(start_block)) {
      blocks_seen.insert(iterator_.BlockStart(nei));
      bfs_frontier.push(std::move(nei));
    }

    while (!bfs_frontier.empty()) {
      auto curr_block = bfs_frontier.front();
      bfs_frontier.pop();
      GetEntitiesFromBlock(entity, iterator_.BlockStart(curr_block), curr_block);
      for (auto nei : iterator_.GetNei(curr_block)) {
        auto nei_start = iterator_.BlockStart(nei);
        if (blocks_seen.find(nei_start) != blocks_seen.end()) {
          continue;
        }
        blocks_seen.insert(nei_start);
        bfs_frontier.push(std::move(nei));
      }
    }

    if (blocks_seen.find(iterator_.BlockStart(start_block)) == blocks_seen.end()) {
      GetEntitiesFromBlock(entity, iterator_.NextStmt(src), start_block);
    }
  }
  void GetEntitiesFromBlock(Entity *src, int start, std::shared_ptr<cfg::Block> &block) {
    std::vector<Entity *> entities;
    int end = iterator_.NextStmt(iterator_.BlockEnd(block));
    for (int stmt = start; stmt != end; stmt = iterator_.NextStmt(stmt)) {
      auto *entity = GetStatement(stmt, pkb_);
      if (include_dest_(entity)) {
        entities.push_back(entity);
        process_row_(src, entity);
      }
    }
  }

  const std::vector<Entity *> &sources_;
  BlockIterator iterator_;
  DestPredicate include_dest_;
  ProcessRow process_row_;
  QueryFacade &pkb_;
};

template<typename BlockIterator>
auto FindReachableEntities(int src, EntityType dest, QueryFacade &pkb) -> std::vector<Entity *> {
  auto *entity = GetStatement(src, pkb);
  if (entity == nullptr) {
    return {};
  }
  std::vector<Entity *> dest_entities;
  ReachableEntityFinder({entity},
                        BlockIterator{},
                        [&](Entity *entity) { return MatchesEntityType(entity, dest); },
                        [&](Entity *src, Entity *dst) { dest_entities.push_back(dst); },
                        pkb)();
  return dest_entities;
}

template<typename BlockIterator>
auto FindReachableEntities(const std::vector<Entity *> src,
                           EntityType dest_type,
                           QueryFacade &pkb) -> std::vector<std::vector<Entity *>> {
  std::vector<std::vector<Entity *>> relationships;
  ReachableEntityFinder(src,
                        BlockIterator{},
                        [&](Entity *entity) { return MatchesEntityType(entity, dest_type); },
                        [&](Entity *src, Entity *dst) { relationships.push_back({src, dst}); },
                        pkb)();
  return relationships;
}

template<typename BlockIterator>
auto Reachable(int src, int dest, QueryFacade &pkb) -> bool {
  auto *entity = GetStatement(src, pkb);
  bool found = false;
  if (entity == nullptr) {
    return found;
  }
  ReachableEntityFinder({entity},
                        BlockIterator{},
                        [dest](Entity *entity) { return GetStmtNo(entity) == dest; },
                        [&](Entity *src, Entity *dst) { found = true; },
                        pkb)();
  return found;
}

}  // namespace qps
