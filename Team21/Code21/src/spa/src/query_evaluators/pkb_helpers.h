#pragma once

#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "query/design_entity.h"
#include "query/synonym.h"
#include "query/declaration.h"
#include "PKBStorageClasses/EntityClasses/Statement.h"
#include "PKB/QueryFacade.h"

namespace qps {
auto DesignEntityToEntityType(DesignEntity entity) -> EntityType;
auto GetEntityType(Synonym &syn, std::vector<Declaration> &declarations) -> EntityType;
auto GetStatement(int stmt_no, QueryFacade &pkb) -> Statement *;
auto GetStmtNo(Entity *entity) -> int;
auto GetEntities(Synonym &syn, std::vector<Declaration> &declarations, QueryFacade &pkb) -> std::vector<Entity *> *;
auto MatchesEntityType(Entity *entity, EntityType type) -> bool;
//template<typename Condition>
//auto FindEntitiesMatchingCondition(QueryFacade &pkb, EntityType type) -> std::vector<Entity *> {
//  auto *entities = pkb.getEntitiesByType(type);
//  std::vector<Entity *> filtered_entities;
//  std::copy_if(entities->begin(), entities->end(), std::back_inserter(filtered_entities),
//               [&](Entity *entity) { return Condition(entity); });
//  return filtered_entities;
//}

} // qps
