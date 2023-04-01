#pragma once

#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

#include "query/design_entity.h"
#include "query/relationship.h"
#include "query/synonym.h"
#include "query/declaration.h"

namespace qps {
auto DesignEntityToEntityType(DesignEntity entity) -> const EntityType &;
auto RelationshipToRelationshipType(Relationship relationship) -> const RelationshipType &;
auto ExtractEntities(const std::vector<::Relationship *> &relationships,
                     bool left,
                     bool right) -> std::vector<std::vector<Entity *>>;

} // qps
