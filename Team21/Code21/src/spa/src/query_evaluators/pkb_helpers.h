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
} // qps
