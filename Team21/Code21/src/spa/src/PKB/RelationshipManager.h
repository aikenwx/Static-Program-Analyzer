//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_RELATIONSHIPMANAGER_H
#define SPA_RELATIONSHIPMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "EntityManager.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "PKB/RelationshipStorage.h"
#include "sp/cfg/cfg.h"



class RelationshipManager {
 private:

  EntityManager *entityManager;

  RelationshipStorage relationshipStorage;

  std::shared_ptr<cfg::CFG> cfg;


  std::vector<Relationship *> emptyRelationshipVector;

  std::vector<Entity *> emptyEntityVector;

 public:
  RelationshipManager(EntityManager* entityManager);

  void storeCFG(std::shared_ptr<cfg::CFG> cfg);

  void storeRelationship(const std::shared_ptr<Relationship> &relationship);

  auto getRelationship(RelationshipKey &key) -> Relationship *;

  auto getRelationshipsByTypes(const RelationshipType &relationshipType,
                               const EntityType &leftHandEntityType,
                               const EntityType &rightHandEntityType)
      -> std::vector<Relationship *> *;

  auto getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(
      RelationshipType &relationshipType, const EntityType &leftHandEntityType,
      EntityKey &rightHandEntityKey) -> std::vector<Entity *> *;
  auto getEntitiesForGivenRelationshipTypeAndRightHandEntityType(
      RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
      const EntityType &rightHandEntityType) -> std::vector<Entity *> *;
};

#endif  // SPA_RELATIONSHIPMANAGER_H
