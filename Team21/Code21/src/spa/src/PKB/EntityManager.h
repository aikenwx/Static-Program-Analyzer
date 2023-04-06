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
 private:
    static const int MAX_NUMBER_OF_STMTS_WITH_BUFFER = 600;
  std::unordered_map<EntityKey, std::shared_ptr<Entity>> entityStore;
  std::unordered_map<EntityType, std::shared_ptr<std::vector<Entity*>>>
      entityTypeToEntityStore;
  std::vector<Entity*> emptyEntityVector;

  // we need this because unorderedmap access is way too slow, and since we are accessing individual stmts often
  // there is quite a lot of overhead
  std::vector<Statement*> fastAccessStmts;

  int numberOfStatements = 0;

 public:
  EntityManager();

  void storeEntity(const std::shared_ptr<Entity>& entity);

  auto getEntity(EntityKey& key) -> Entity*;

  auto getEntitiesByType(const EntityType& entityType) -> std::vector<Entity*>*;

  auto getNumberOfStatements() const -> int;

  
  auto getStmtByNumber(int stmtNumber) -> Statement*;

 private:
  void storeInEntityTypeStore(Entity* entity);

  void initialiseVectorForEntityTypeStoreIfIndexNotExist(
      const EntityType& entityType);
};

#endif  // SPA_ENTITYMANAGER_H
