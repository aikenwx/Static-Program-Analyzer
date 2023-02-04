//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_ENTITYMANAGER_H
#define SPA_ENTITYMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../PKBStorageClasses/EntityClasses/Entity.h"
#include "../PKBStorageClasses/EntityClasses/Constant.h"
#include "../PKBStorageClasses/EntityClasses/Variable.h"
#include "../PKBStorageClasses/EntityClasses/Procedure.h"
#include "../PKBStorageClasses/EntityClasses/Statement.h"
#include "../PKBStorageClasses/EntityClasses/IfStatement.h"
#include "../PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "../PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "../PKBStorageClasses/EntityClasses/CallStatement.h"
#include "../PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "../PKBStorageClasses/EntityClasses/PrintStatement.h"

class EntityManager {
private:
    std::unordered_map<int, std::vector<Entity *> *> *entityMap;

public:
    EntityManager();

    ~EntityManager();

    void storeEntity(Entity *entity);

    std::vector<Entity *> *getEntitiesByType(EntityType entityType);


};

#endif //SPA_ENTITYMANAGER_H
