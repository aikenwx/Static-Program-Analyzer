//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_ENTITYMANAGER_H
#define SPA_ENTITYMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "../PKBStorageClasses/EntityClasses/CallStatement.h"
#include "../PKBStorageClasses/EntityClasses/Constant.h"
#include "../PKBStorageClasses/EntityClasses/Entity.h"
#include "../PKBStorageClasses/EntityClasses/IfStatement.h"
#include "../PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "../PKBStorageClasses/EntityClasses/Procedure.h"
#include "../PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "../PKBStorageClasses/EntityClasses/Statement.h"
#include "../PKBStorageClasses/EntityClasses/Variable.h"
#include "../PKBStorageClasses/EntityClasses/WhileStatement.h"

class EntityManager {
 public:
  static const int MAX_NUMBER_OF_STMTS_WITH_BUFFER = 600;

 private:
  std::unordered_map<EntityKey, Entity*> entityStore;
  std::unordered_map<EntityType, std::vector<Entity*>*> entityTypeToEntityStore;
  std::vector<Entity*> emptyEntityVector;

  std::unique_ptr<std::vector<Entity*>> placeholderEntityVector;

  std::vector<std::unique_ptr<Entity>> entitiesOwner;
  std::vector<std::unique_ptr<std::vector<Entity*>>> entityVectorOwner;

  // we need this because unorderedmap access is way too slow, and since we are
  // accessing individual stmts often there is quite a lot of overhead
  std::vector<Statement*> fastAccessStmts;

  int numberOfStatements = 0;

 public:
  EntityManager();

  void storeEntity(std::unique_ptr<Entity> entity);

  auto getEntity(EntityKey& key) -> Entity*;

  auto getEntitiesByType(const EntityType& entityType) -> std::vector<Entity*>*;

  auto getNumberOfStatements() const -> int;

  auto getStmtByNumber(int stmtNumber) -> Statement*;

 private:
  void storeInEntityTypeStore(Entity* entity);

  auto getVectorForEntityTypeStore(const EntityType& entityType)
      -> std::vector<Entity*>*;
};

#endif  // SPA_ENTITYMANAGER_H
