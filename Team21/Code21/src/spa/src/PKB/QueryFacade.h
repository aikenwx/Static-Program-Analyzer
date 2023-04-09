//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_QUERYFACADE_H
#define SPA_QUERYFACADE_H

#include <unordered_set>
#include <vector>

#include "PKB/EntityManager.h"
#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Statement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/CallsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/CallsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "sp/cfg/cfg.h"

class QueryFacade {
 private:
  EntityManager* entityManager;
  RelationshipManager* relationshipManager;

 public:
  QueryFacade(EntityManager *entityManager, RelationshipManager *relationshipManager);

  auto getEntity(const EntityType& entityType, int entityValue) -> Entity*;

  auto getEntity(const EntityType& entityType, std::string entityValue) -> Entity*;

  auto getEntitiesByType(const EntityType& entityType) -> std::vector<Entity*>*;

  auto getRelationship(const RelationshipType & relationshipType,
                       const EntityType & leftEntityType, int leftEntityValue,
                       const EntityType & rightEntityType , int rightEntityValue)
      -> Relationship*;

  auto getRelationship(const RelationshipType & relationshipType,
                       const EntityType & leftEntityType, std::string leftEntityValue,
                       const EntityType & rightEntityType ,
                       std::string rightEntityValue) -> Relationship*;

  auto getRelationship(const RelationshipType & relationshipType,
                       const EntityType & leftEntityType, int leftEntityValue,
                       const EntityType & rightEntityType,
                       std::string rightEntityValue) -> Relationship*;

  auto getRelationship(const RelationshipType & relationshipType,
                       const EntityType & leftEntityType, std::string leftEntityValue,
                       const EntityType & rightEntityType, int rightEntityValue)
      -> Relationship*;

  auto getRelationshipsByTypes(const RelationshipType & relationshipType,
                               const EntityType & leftEntityType,
                               const EntityType & rightEntityType)
      -> std::vector<Relationship*>*;

  auto getLeftEntitiesRelatedToGivenRightEntity(
      const RelationshipType & relationshipType, const EntityType & leftEntityType,
      const EntityType & rightEntityType, int rightEntityValue)
      -> std::vector<Entity*>*;

  auto getLeftEntitiesRelatedToGivenRightEntity(
      const RelationshipType & relationshipType, const EntityType & leftEntityType,
      const EntityType & rightEntityType, std::string rightEntityValue)
      -> std::vector<Entity*>*;

  auto getRightEntitiesRelatedToGivenLeftEntity(
          const RelationshipType & relationshipType, const EntityType & leftEntityType,
          int statementNumber, const EntityType & rightEntityType)
      -> std::vector<Entity*>*;

  auto getRightEntitiesRelatedToGivenLeftEntity(
      const RelationshipType & relationshipType, const EntityType & leftEntityType,
      std::string leftEntityValue, const EntityType & rightEntityType)
      -> std::vector<Entity*>*;

  void clearCache();
};

#endif  // SPA_QUERYFACADE_H
