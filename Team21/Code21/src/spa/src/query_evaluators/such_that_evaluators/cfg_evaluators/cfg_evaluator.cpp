#include "cfg_evaluator.h"

namespace qps {
//auto Next(cfg::CFG &cfg, int stmt_no) -> std::vector<int> {
//  auto opt_block = cfg.GetBlockAt(stmt_no);
//  if (!opt_block) {
//    return {};
//  }
//  auto &block = opt_block.value();
//  std::vector<int> neighbors;
//  if (block->IsInBlock(stmt_no + 1)) {
//    neighbors.push_back(stmt_no + 1);
//  } else {
//    for (const auto &child : block->children()) {
//      neighbors.push_back(child->start());
//    }
//  }
//  return neighbors;
//}

//auto Next(cfg::CFG &cfg, Entity *entity) -> std::vector<int> {
//
//  auto *statement = dynamic_cast<Statement *>(entity);
//  return Next(cfg, statement->getStatementNumber());
//}
//
//auto Prev(cfg::CFG &cfg, int stmt_no) -> std::vector<int> {
//  auto opt_block = cfg.GetBlockAt(stmt_no);
//  if (!opt_block) {
//    return {};
//  }
//  auto &block = opt_block.value();
//  std::vector<int> neighbors;
//  if (block->IsInBlock(stmt_no - 1)) {
//    neighbors.push_back(stmt_no - 1);
//  } else {
//    for (const auto &parent : block->parents()) {
//      neighbors.push_back(parent->end());
//    }
//  }
//  return neighbors;
//}
//
//auto Prev(cfg::CFG &cfg, Entity *entity) -> std::vector<int> {
//  auto *statement = dynamic_cast<Statement *>(entity);
//  return Prev(cfg, statement->getStatementNumber());
//}

} // qps
