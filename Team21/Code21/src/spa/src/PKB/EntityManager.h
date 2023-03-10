//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_ENTITYMANAGER_H
#define SPA_ENTITYMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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
    std::unordered_map<EntityKey, std::shared_ptr<Entity>> entityStore;
    std::unordered_map<EntityType, std::shared_ptr<std::vector<Entity*>>> entityTypeToStore;

   public:
    EntityManager();

    ~EntityManager();

    void storeEntity(Entity* entity);

    Entity* getEntity(EntityKey& key);

    std::vector<Entity*>* getEntitiesByType(EntityType& entityType);

   private:
    void storeInEntityTypeStore(Entity* entity);

    void initialiseVectorForEntityTypeStoreIfIndexNotExist(EntityType& entityType);
};

#endif  // SPA_ENTITYMANAGER_H
