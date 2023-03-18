#pragma once

#include <queue>
#include <unordered_set>

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

struct ForwardBlockIterator {
  static auto NextStmt(int stmt_no) -> int {
    return stmt_no + 1;
  }

  static auto GetNei(std::shared_ptr<cfg::Block> &block) -> std::vector<std::shared_ptr<cfg::Block>> {
    return block->children();
  }

  static auto BlockStart(std::shared_ptr<cfg::Block> &block) -> int {
    return block->start();
  }

  static auto BlockEnd(std::shared_ptr<cfg::Block> &block) -> int {
    return block->end();
  }

  static auto GoingForward(int curr_stmt, int next_stmt) -> bool {
    return next_stmt > curr_stmt;
  }
};

struct ReverseBlockIterator {
  static auto NextStmt(int stmt_no) -> int {
    return stmt_no - 1;
  }

  static auto GetNei(std::shared_ptr<cfg::Block> &block) -> std::vector<std::shared_ptr<cfg::Block>> {
    return block->parents();
  }

  static auto BlockStart(std::shared_ptr<cfg::Block> &block) -> int {
    return block->end();
  }

  static auto BlockEnd(std::shared_ptr<cfg::Block> &block) -> int {
    return block->start();
  }
  static auto GoingForward(int curr_stmt, int next_stmt) -> bool {
    return next_stmt < curr_stmt;
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

template<typename BlockIterator, typename DestPredicate>
auto GetEntitiesFromBlock(int src_stmt,
                          std::shared_ptr<cfg::Block> &block,
                          BlockIterator iterator,
                          DestPredicate include_dest,
                          QueryFacade &pkb) -> std::vector<Entity *> {
  std::vector<Entity *> entities;
  int end = iterator.NextStmt(iterator.BlockEnd(block));
  for (int stmt = src_stmt; stmt != end; stmt = iterator.NextStmt(stmt)) {
    auto *statement = GetStatement(stmt, pkb);
    if (include_dest(statement)) {
      entities.push_back(statement);
    }
  }
  return entities;
}

template<typename BlockIterator>
auto IsWhileChild(std::shared_ptr<cfg::Block> &block,
                  std::shared_ptr<cfg::Block> &parent,
                  BlockIterator iterator) -> bool {
  for (auto &nei : iterator.GetNei(block)) {
    if (!iterator.GoingForward(iterator.BlockEnd(block), iterator.BlockStart(nei))
        && iterator.BlockStart(nei) == iterator.BlockStart(parent)) {
      return true;
    }
  }
  return false;
}

template<typename DestPredicate, typename BlockIterator, typename ProcessRow>
void FindReachableEntities(const std::vector<Entity *> &sources,
                           BlockIterator iterator,
                           DestPredicate include_dest,
                           ProcessRow process_row,
                           QueryFacade &pkb) {
  std::unordered_map<int, std::vector<Entity *>> block_cache;
  std::unordered_map<int, std::vector<Entity *>> while_child_cache;
  std::function<void(std::shared_ptr<cfg::Block> &block)> reachable_from;
  std::function<void(std::shared_ptr<cfg::Block> &block)> get_while_child_entities;

  reachable_from = [&](std::shared_ptr<cfg::Block> &block) {
    if (block_cache.find(iterator.BlockStart(block)) != block_cache.end()) {
      return;
    }
    std::vector<Entity *> entities_reachable = GetEntitiesFromBlock(iterator.BlockStart(block), block,
                                                                    iterator, include_dest, pkb);
    for (auto &nei : iterator.GetNei(block)) {
      if (IsWhileChild(nei, block, iterator)) {
        auto statement = GetStatement(iterator.BlockStart(block), pkb);
        if (include_dest(statement)) {
          entities_reachable.push_back(statement);
        }
        get_while_child_entities(nei);
        auto iter = while_child_cache.find(iterator.BlockStart(nei));
        entities_reachable.insert(entities_reachable.end(), iter->second.begin(), iter->second.end());
        continue;
      }
      reachable_from(nei);
      auto iter = block_cache.find(iterator.BlockStart(nei));
      entities_reachable.insert(entities_reachable.begin(), iter->second.begin(), iter->second.end());
    }
    block_cache[iterator.BlockStart(block)] = std::move(entities_reachable);
  };

  get_while_child_entities = [&](std::shared_ptr<cfg::Block> &block) {
    if (while_child_cache.find(iterator.BlockStart(block)) != while_child_cache.end()) {
      return;
    }
    std::vector<Entity *> entities_reachable = GetEntitiesFromBlock(iterator.BlockStart(block), block,
                                                                    iterator, include_dest, pkb);
    for (auto &nei : iterator.GetNei(block)) {
      if (!iterator.GoingForward(iterator.BlockEnd(block), iterator.BlockStart(nei))) {
        continue;
      }
      reachable_from(nei);
      auto iter = block_cache.find(iterator.BlockStart(nei));
      entities_reachable.insert(entities_reachable.begin(), iter->second.begin(), iter->second.end());
    }
    while_child_cache[iterator.BlockStart(block)] = std::move(entities_reachable);
  };

  auto &cfg = *pkb.getCFG();
  for (const auto &source : sources) {
    int stmt = GetStmtNo(source);
    auto opt_start = cfg.GetBlockAt(stmt);
    if (!opt_start) {
      continue;
    }
    auto start_block = opt_start.value();
    for (auto &nei : iterator.GetNei(start_block)) {
      reachable_from(nei);
      for (auto &dest : block_cache[iterator.BlockStart(nei)]) {
        process_row(source, dest);
      }
    }
    if (stmt != iterator.BlockEnd(start_block)) {
      for (auto &dest : GetEntitiesFromBlock(iterator.NextStmt(stmt), start_block,
                                             iterator, include_dest, pkb)) {
        process_row(source, dest);
      }
    }
  }
}

template<typename BlockIterator>
auto FindReachableEntities(int src, EntityType dest, QueryFacade &pkb) -> std::vector<Entity *> {
  auto *entity = GetStatement(src, pkb);
  if (entity == nullptr) {
    return {};
  }
  std::vector<Entity *> dest_entities;
  FindReachableEntities({entity},
                        BlockIterator{},
                        [&](Entity *entity) { return MatchesEntityType(entity, dest); },
                        [&](Entity *src, Entity *dst) { dest_entities.push_back(dst); },
                        pkb);
  return dest_entities;
}

template<typename BlockIterator>
auto FindReachableEntities(const std::vector<Entity *> src,
                           EntityType src_type,
                           EntityType dest_type,
                           QueryFacade &pkb) -> std::vector<std::vector<Entity *>> {
  std::vector<std::vector<Entity *>> relationships;
  FindReachableEntities(src,
                        BlockIterator{},
                        [&](Entity *entity) { return MatchesEntityType(entity, dest_type); },
                        [&](Entity *src, Entity *dst) { relationships.push_back({src, dst}); },
                        pkb);
  return relationships;
}

template<typename BlockIterator>
auto Reachable(int src, int dest, QueryFacade &pkb) -> bool {
  auto *entity = GetStatement(src, pkb);
  bool found = false;
  if (entity == nullptr) {
    return found;
  }
  FindReachableEntities({entity},
                        BlockIterator{},
                        [dest](Entity *entity) { return GetStmtNo(entity) == dest; },
                        [&](Entity *src, Entity *dst) { found = true; },
                        pkb);
  return found;
}

}  // namespace qps
