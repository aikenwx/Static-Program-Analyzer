#pragma once

#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

#include "query/design_entity.h"
#include "query/relationship.h"
#include "query/synonym.h"
#include "query/declaration.h"

namespace qps {
auto DesignEntityToEntityType(DesignEntity entity) -> EntityType;
auto RelationshipToRelationshipType(Relationship relationship) -> RelationshipType;
//auto GetEntityType(Synonym &syn, const std::vector<Declaration> &declarations) -> EntityType;
auto ExtractEntities(const std::vector<::Relationship *> &relationships) -> std::vector<std::vector<Entity *>>;

} // qps
