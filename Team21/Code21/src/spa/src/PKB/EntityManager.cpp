//
// Created by Aiken Wong on 3/2/23.
//

#include "EntityManager.h"

#include <iostream>

EntityManager::EntityManager() {
    this->entityMap = std::unordered_map<int, std::shared_ptr<std::vector<std::shared_ptr<Entity>>>>();
}

EntityManager::~EntityManager() {
    this->entityMap.clear();
}

void EntityManager::storeEntity(Entity* entity) {
    initialiseVectorForIndexIfNotExist(entity->getEntityType());
    this->entityMap.at(entity->getEntityType())->push_back(std::shared_ptr<Entity>(entity));
}

std::vector<std::shared_ptr<Entity>>* EntityManager::getEntitiesByType(EntityType entityType) {
    if (entityType == EntityType::STATEMENT) {
        return this->getAllStatements();
    }
    // initialise the vector if it does not exist
    EntityManager::initialiseVectorForIndexIfNotExist(entityType);
    return this->entityMap.at(entityType).get();
}

std::vector<std::shared_ptr<Entity>>* EntityManager::getAllStatements() {
    if (this->entityMap.find(EntityType::STATEMENT) != this->entityMap.end()) {
        return this->entityMap.at(EntityType::STATEMENT).get();
    }

    std::shared_ptr<std::vector<std::shared_ptr<Entity>>> statementVector = std::make_shared<std::vector<std::shared_ptr<Entity>>>();

    for (EntityType statementType : Entity::statementTypes) {
        EntityManager::initialiseVectorForIndexIfNotExist(statementType);
        statementVector->insert(statementVector->end(), this->entityMap.at(statementType)->begin(), this->entityMap.at(statementType)->end());
    }
    this->entityMap.insert({EntityType::STATEMENT, statementVector});
    return statementVector.get();
}

void EntityManager::initialiseVectorForIndexIfNotExist(EntityType entityType) {
    if (this->entityMap.find(entityType) == this->entityMap.end()) {
        this->entityMap.insert({entityType, std::make_shared<std::vector<std::shared_ptr<Entity>>>()});
    }
}
