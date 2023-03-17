#include "cfg_evaluator.h"

namespace qps {
auto GetStmtNo(Entity *entity) -> int {
  return dynamic_cast<Statement *>(entity)->getStatementNumber();
}

auto GetEntity(int stmt_no, QueryFacade &pkb) -> Entity * {
  return pkb.getEntity(Statement::getEntityTypeStatic(), stmt_no);
}

auto GetEntities(Synonym &syn, std::vector<Declaration> &declarations, QueryFacade &pkb) -> std::vector<Entity *> * {
  auto entity_type = GetEntityType(syn, declarations);
  return pkb.getEntitiesByType(entity_type);
}

auto MatchesEntityType(Entity *entity, EntityType type) -> bool {
  if (type == Statement::getEntityTypeStatic()) {
    return Statement::isStatement(entity);
  } else {
    return entity->getEntityType() == type;
  }
}

auto Next(cfg::CFG &cfg, int stmt_no) -> std::vector<int> {
  auto opt_block = cfg.GetBlockAt(stmt_no);
  if (!opt_block) {
    return {};
  }
  auto &block = opt_block.value();
  std::vector<int> neighbors;
  if (block->IsInBlock(stmt_no + 1)) {
    neighbors.push_back(stmt_no + 1);
  } else {
    for (const auto &child : block->children()) {
      neighbors.push_back(child->start());
    }
  }
  return neighbors;
}

auto Next(cfg::CFG &cfg, Entity *entity) -> std::vector<int> {
  auto *statement = dynamic_cast<Statement *>(entity);
  return Next(cfg, statement->getStatementNumber());
}

auto Prev(cfg::CFG &cfg, int stmt_no) -> std::vector<int> {
  auto opt_block = cfg.GetBlockAt(stmt_no);
  if (!opt_block) {
    return {};
  }
  auto &block = opt_block.value();
  std::vector<int> neighbors;
  if (block->IsInBlock(stmt_no - 1)) {
    neighbors.push_back(stmt_no - 1);
  } else {
    for (const auto &parent : block->parents()) {
      neighbors.push_back(parent->end());
    }
  }
  return neighbors;
}

auto Prev(cfg::CFG &cfg, Entity *entity) -> std::vector<int> {
  auto *statement = dynamic_cast<Statement *>(entity);
  return Prev(cfg, statement->getStatementNumber());
}

auto Reachable(int src, int dest, QueryFacade &pkb) -> bool {
  auto *entity = GetEntity(src, pkb);
  bool found = false;
  FindReachableEntities({entity},
                        BlockChildren,
                        [src](Entity *entity) { return GetStmtNo(entity) == src; },
                        [dest](Entity *entity) { return GetStmtNo(entity) == dest; },
                        [&](Entity *src, Entity *dst) { found = true; },
                        pkb);
  return found;
}

} // qps
