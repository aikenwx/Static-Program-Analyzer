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
    std::unordered_map<int, std::shared_ptr<std::vector<std::shared_ptr<Entity>>>> entityMap;

   public:
    EntityManager();

    ~EntityManager();

    void storeEntity(Entity *entity);

    std::vector<std::shared_ptr<Entity>> *getEntitiesByType(EntityType entityType);

   private:
    void initialiseVectorForIndexIfNotExist(EntityType entityType);

    std::vector<std::shared_ptr<Entity>> *getAllStatements();
};

#endif  // SPA_ENTITYMANAGER_H
