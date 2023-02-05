//
// Created by Aiken Wong on 3/2/23.
//

#include "EntityManager.h"

EntityManager::EntityManager() {
    this->entityMap = new std::unordered_map<int, std::vector<Entity *> *>();
}

EntityManager::~EntityManager() {

    // todo delete all the vector in the hash map

    delete this->entityMap;
}

void EntityManager::storeEntity(Entity *entity) {

    // initialise the vector if it does not exist
    if (this->entityMap->find(entity->getEntityType()) == this->entityMap->end()) {
        this->entityMap->insert({entity->getEntityType(), new std::vector<Entity *>()});
    }

    this->entityMap->at(entity->getEntityType())->push_back(entity);
}

std::vector<Entity *> *EntityManager::getEntitiesByType(EntityType entityType) {
    if (entityType == EntityType::STATEMENT) {
        return this->getAllStatements();
    }
    // initialise the vector if it does not exist
    EntityManager::initialiseVectorForIndexIfNotExist(entityType);
    return this->entityMap->at(entityType);
}

std::vector<Entity *> *EntityManager::getAllStatements() {
    std::vector<Entity *> *statementVector = new std::vector<Entity *>();

    for (EntityType statementType : Entity::statementTypes) {
        EntityManager::initialiseVectorForIndexIfNotExist(statementType);

        statementVector->insert(statementVector->end(), this->entityMap->at(statementType)->begin(), this->entityMap->at(statementType)->end());
    }

    return statementVector;
}

void EntityManager::initialiseVectorForIndexIfNotExist(EntityType entityType) {
    if (this->entityMap->find(entityType) == this->entityMap->end()) {
        this->entityMap->insert({entityType, new std::vector<Entity *>()});
    }
}




