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
  int next_possible_statment = BlockIterator::NextStmt(stmt_no + 1);
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

//auto Next(cfg::CFG &cfg, int stmt_no) -> std::vector<int>;
//auto Next(cfg::CFG &cfg, Entity *entity) -> std::vector<int>;
//auto Prev(cfg::CFG &cfg, int stmt_no) -> std::vector<int>;
//auto Prev(cfg::CFG &cfg, Entity *entity) -> std::vector<int>;

template<typename T>
auto HasNext(cfg::CFG &cfg, T stmt) -> bool {
  return !DirectNeighbors<ForwardBlockIterator>(cfg, stmt).empty();
}

template<typename T>
auto HasPrev(cfg::CFG &cfg, T stmt) -> bool {
  return !DirectNeighbors<ReverseBlockIterator>(cfg, stmt).empty();
}

template<typename SrcPredicate, typename DestPredicate, typename BlockIterator, typename ProcessRow>
class ReachableEntityFinder {
 public:
  ReachableEntityFinder(const std::vector<Entity *> &sources,
                        BlockIterator iterator,
                        SrcPredicate include_src,
                        DestPredicate include_dest,
                        ProcessRow process_row,
                        QueryFacade &pkb)
      : sources_(sources), include_src_(include_src), include_dest_(include_dest),
        process_row_(process_row), pkb_(pkb), iterator_(iterator) {}

  void operator()() {
    auto cfg = pkb_.getCFG();
    for (const auto &source : sources_) {
      Search(source, *cfg);
    }
    ProcessRelationships();
  }
 private:
  // Does a breadth first search until a statement that could be another src is reached.
  // Search is terminated then.
  void Search(Entity *src, cfg::CFG &cfg) {
    int source_stmt_no = GetStmtNo(src);
    auto opt_start = cfg.GetBlockAt(source_stmt_no);
    if (!opt_start) {
      return;
    }
    relationships_[src] = {};
    auto start_block = opt_start.value();
    std::queue<std::shared_ptr<cfg::Block>> blocks;
    blocks.push(start_block);
    std::unordered_map<int, int> block_seen = {{iterator_.BlockStart(start_block), 1}};
    while (!blocks.empty()) {
      auto curr_block = blocks.front();
      blocks.pop();
      auto curr_block_start = iterator_.BlockStart(curr_block);
      if (curr_block->IsInBlock(source_stmt_no) && block_seen[iterator_.BlockStart(start_block)] == 1) {
        curr_block_start = iterator_.NextStmt(source_stmt_no);
      }

      if (!ProcessBlock(src, curr_block_start, curr_block)) {
        continue;
      }

      for (auto &nei : iterator_.GetNei(curr_block)) {
        auto block_start = iterator_.BlockStart(nei);
        if (block_seen[block_start] == 2 ||
            (iterator_.GoingForward(iterator_.BlockEnd(curr_block), block_start) && block_seen[block_start] == 1)) {
          continue;
        }
        block_seen[block_start] += 1;
        blocks.push(nei);
      }
    }
  }

  auto ProcessBlock(Entity *src, int start, std::shared_ptr<cfg::Block> &block) -> bool {
    auto curr_block_end = iterator_.BlockEnd(block);
    for (int stmt = start; stmt != iterator_.NextStmt(curr_block_end); stmt = iterator_.NextStmt(stmt)) {
      auto *stmt_entity = GetStatement(stmt, pkb_);
      if (include_dest_(stmt_entity)) {
        relationships_[src].push_back(stmt_entity);
      }
      if (include_src_(stmt_entity)) {
        if (stmt_entity != src) {
          in_degree_[src] += 1;
          terminated_at_[stmt_entity].push_back(src);
        }
        return false;
      }
    }
    return true;
  }

  // Does a Topo Sort
  void ProcessRelationships() {
    std::queue<Entity *> frontier;
    for (auto &pair : relationships_) {
      if (in_degree_[pair.first] == 0) {
        frontier.push(pair.first);
      }
      for (auto &dest : pair.second) {
        process_row_(pair.first, dest);
      }
    }

    while (!frontier.empty()) {
      Entity *entity = frontier.front();
      frontier.pop();
      for (auto &affected_entity : terminated_at_[entity]) {
        for (auto &dest : relationships_[entity]) {
          process_row_(affected_entity, dest);
          relationships_[affected_entity].push_back(dest);
        }
        in_degree_[affected_entity] -= 1;
        if (in_degree_[affected_entity] == 0) {
          frontier.push(affected_entity);
        }
      }
    }
  }

  QueryFacade &pkb_;
  BlockIterator iterator_;
  SrcPredicate include_src_;
  DestPredicate include_dest_;
  ProcessRow process_row_;
  const std::vector<Entity *> &sources_;
  std::unordered_map<Entity *, int> in_degree_;
  std::unordered_map<Entity *, std::vector<Entity *>> terminated_at_;
  std::unordered_map<Entity *, std::vector<Entity *>> relationships_;
};
//
//template<typename SrcPredicate, typename DestPredicate, typename BlockIterator, typename ProcessRow>
//auto FindReachableEntities(const std::vector<Entity *> &sources,
//                           BlockIterator iterator,
//                           SrcPredicate include_src,
//                           DestPredicate include_dest,
//                           ProcessRow process_row,
//                           QueryFacade &pkb) {
//  auto *cfg = pkb.getCFG();
//  std::unordered_map<Entity *, std::vector<Entity *>> terminated_at;
//  std::unordered_map<Entity *, std::vector<Entity *>> relationships;
//
//  for (const auto &source : sources) {
//    int source_stmt_no = GetStmtNo(source);
//    auto opt_start = cfg->GetBlockAt(source_stmt_no);
//    if (!opt_start) {
//      continue;
//    }
//    auto start_block = opt_start.value();
//    std::queue<std::shared_ptr<cfg::Block>> blocks;
//    blocks.push(start_block);
//    std::unordered_map<int, int> block_seen = {{iterator.BlockStart(start_block), 1}};
//    while (!blocks.empty()) {
//      auto curr_block = blocks.front();
//      blocks.pop();
//      auto curr_block_start = iterator.BlockStart(curr_block);
//      auto curr_block_end = iterator.BlockEnd(curr_block);
//      if (curr_block->IsInBlock(source_stmt_no) && block_seen[iterator.BlockStart(start_block)] == 1) {
//        curr_block_start = iterator.NextStmt(source_stmt_no);
//      }
//
//      bool discontinue = false;
//      for (int stmt = curr_block_start; stmt != iterator.NextStmt(curr_block_end);
//           stmt = iterator.NextStmt(stmt)) {
//        auto *stmt_entity = GetStatement(stmt, pkb);
//        if (include_dest(stmt_entity)) {
//          relationships[source].push_back(stmt_entity);
//        }
//        if (include_src(stmt_entity)) {
//          if (stmt_entity != source) {
//            terminated_at[source].push_back(stmt_entity);
//          }
//          discontinue = true;
//          break;
//        }
//      }
//      if (discontinue) {
//        break;
//      }
//
//      for (auto &nei : iterator.GetNei(curr_block)) {
//        auto block_start = iterator.BlockStart(nei);
//        if (block_seen[block_start] == 2 ||
//            (iterator.GoingForward(curr_block_end, block_start) && block_seen[block_start] == 1)) {
//          continue;
//        }
//        block_seen[block_start] += 1;
//        blocks.push(nei);
//      }
//    }
//  }
//
//  for (auto &relationship : relationships) {
//    for (auto &dest : relationship.second) {
//      process_row(relationship.first, dest);
//    }
//  }
//
//  for (auto &srcs : terminated_at) {
//    for (auto &reachable_src : srcs.second) {
//      for (auto &dest : relationships[reachable_src]) {
//        process_row(srcs.first, dest);
//      }
//    }
//  }
//}



//template<typename SrcPredicate, typename DestPredicate, typename BlockIterator, typename ProcessRow>
//void (const std::vector<Entity *> &sources,
//                           BlockIterator iterator,
//                           SrcPredicate include_src,
//                           DestPredicate include_dest,
//                           ProcessRow process,
//                           QueryFacade &pkb) {
//  auto *cfg = pkb.getCFG();
//  std::unordered_set<int> seen;
//  for (const auto &source : sources) {
//    int source_stmt_no = GetStmtNo(source);
//    if (seen.find(source_stmt_no) != seen.end()) {
//      continue;
//    }
//    auto opt_start = cfg->GetBlockAt(source_stmt_no);
//    if (!opt_start) {
//      continue;
//    }
//    std::unordered_set<Entity *> active_src{source};
//    auto start_block = opt_start.value();
//    std::unordered_map<int, int> block_seen = {{iterator.BlockStart(start_block), 1}};
//    std::queue<std::shared_ptr<cfg::Block>> blocks;
//    blocks.push(start_block);
//    while (!blocks.empty()) {
//      auto curr_block = blocks.front();
//      blocks.pop();
//      auto curr_block_start = iterator.BlockStart(curr_block);
//      auto curr_block_end = iterator.BlockEnd(curr_block);
//      if (curr_block->IsInBlock(source_stmt_no) && block_seen[iterator.BlockStart(start_block)] == 1) {
//        curr_block_start = iterator.NextStmt(source_stmt_no);
//      }
//      for (int stmt = curr_block_start; stmt != iterator.NextStmt(curr_block_end);
//           stmt = iterator.NextStmt(stmt)) {
//        auto *stmt_entity = GetStatement(stmt, pkb);
//        if (include_dest(stmt_entity)) {
//          for (auto *src : active_src) {
//            std::cout << GetStmtNo(src) << " " << stmt << "\n";
//            process(src, stmt_entity);
//          }
//        }
//        if (include_src(stmt_entity)) {
//          active_src.insert(stmt_entity);
//          seen.insert(stmt);
//        }
//      }
//      for (auto &nei : iterator.GetNei(curr_block)) {
//        auto block_start = iterator.BlockStart(nei);
//        if (block_seen[block_start] == 2 ||
//            (iterator.GoingForward(curr_block_end, block_start) && block_seen[block_start] == 1)) {
//          continue;
//        }
//        block_seen[block_start] += 1;
//        blocks.push(nei);
//      }
//    }
//  }
//}

template<typename BlockIterator>
auto FindReachableEntities(int src, EntityType dest, QueryFacade &pkb) -> std::vector<Entity *> {
  auto *entity = GetStatement(src, pkb);
  if (entity == nullptr) {
    return {};
  }
  std::vector<Entity *> dest_entities;
  ReachableEntityFinder({entity},
                        BlockIterator{},
                        [](Entity *entity) { return false; },
                        [&](Entity *entity) { return MatchesEntityType(entity, dest); },
                        [&](Entity *src, Entity *dst) { dest_entities.push_back(dst); },
                        pkb)();
  return dest_entities;
}

template<typename BlockIterator>
auto FindReachableEntities(const std::vector<Entity *> src,
                           EntityType src_type,
                           EntityType dest_type,
                           QueryFacade &pkb) -> std::vector<std::vector<Entity *>> {
  std::vector<std::vector<Entity *>> relationships;
  ReachableEntityFinder(src,
                        BlockIterator{},
                        [&](Entity *entity) { return MatchesEntityType(entity, src_type); },
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
                        [src](Entity *entity) { return false; },
                        [dest](Entity *entity) { return GetStmtNo(entity) == dest; },
                        [&](Entity *src, Entity *dst) { found = true; },
                        pkb)();
  return found;
}

}  // namespace qps
