//
// Created by Aiken Wong on 26/3/23.
//

#ifndef SPA_CFGRELATIONSHIPEVALUATOR_H
#define SPA_CFGRELATIONSHIPEVALUATOR_H

#include "PKB/RelationshipStorage.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "sp/cfg/cfg.h"

class CFGRelationshipEvaluator {
 private:
  cfg::CFG* cfg;
  RelationshipStorage* relationshipStorage;
  EntityManager* entityManager;

 public:
  CFGRelationshipEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager);

  auto getCFG() const -> cfg::CFG*;
  auto getRelationshipStorage() const -> RelationshipStorage*;

  virtual auto getRelationshipType() const -> const RelationshipType& = 0;

  // e.g. given left statement = 1, relationship = Next, s = stmt synonym, evaluatuate Next(1, s), caching result and all partial results
  virtual auto evaluateAndCacheRelationshipFromLeftStatement(Statement* leftStatement) -> std::vector<Relationship*>* = 0;

  // e.g. given right statement = 1, relationship = Next, s = stmt synonym, evaluatuats Next(s, 1) caching result and all partial results
  virtual auto evaluateAndCacheRelationshipFromRightStatement(Statement* rightEntity) -> std::vector<Relationship*>* = 0;

  void evaluateAndCacheRelationshipsByEntities(Entity* leftEntity, Entity* rightEntity);

  void evaluateAndCacheRelationshipsByEntityTypes(const EntityType& leftEntityType, const EntityType& rightEntityType);

  void evaluateAndCacheRelationshipsByLeftEntityTypeAndRightEntity(const EntityType& leftEntityType, Entity* rightEntity);

  void evaluateAndCacheRelationshipsByLeftEntityAndRightEntityType(Entity* leftEntity, const EntityType& rightEntityType);
};

#endif  // SPA_CFGRELATIONSHIPEVALUATOR_H
