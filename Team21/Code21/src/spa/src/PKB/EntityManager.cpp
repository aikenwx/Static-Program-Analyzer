//
// Created by Aiken Wong on 3/2/23.
//

#include "EntityManager.h"

#include <stdexcept>

EntityManager::EntityManager() {
    this->entityTypeToStore = std::unordered_map<EntityType, std::shared_ptr<std::vector<Entity *>>>();
    this->entityStore = std::unordered_map<EntityKey, std::shared_ptr<Entity>>();
}

EntityManager::~EntityManager() {
    this->entityTypeToStore.clear();
    this->entityStore.clear();
}

void EntityManager::storeEntity(Entity *entity) {
    // If the entity is a statement, we store it using a statement key
    // This would save us from having to store the same entity twice
    // However, we would not be able to retrieve the entity using the original entity key
    if (Statement::isStatement(entity)) {
        EntityKey statementKey = EntityKey(&Statement::getEntityTypeStatic(), entity->getEntityValue());
        if (entityStore.find(statementKey) != entityStore.end()) {
            delete entity;
            return;
        }

        this->entityStore.insert(std::make_pair(statementKey, std::shared_ptr<Entity>(entity)));
    } else {
        if (entityStore.find(entity->getEntityKey()) != entityStore.end()) {
            return;
        }
        this->entityStore.insert(std::make_pair(entity->getEntityKey(), std::shared_ptr<Entity>(entity)));
    }

    this->storeInEntityTypeStore(entity);
}

Entity *EntityManager::getEntity(EntityKey &key) {
    if (entityStore.find(key) != entityStore.end()) {
        return this->entityStore.at(key).get();
    }

    return nullptr;
}

void EntityManager::storeInEntityTypeStore(Entity *entity) {
    EntityType &entityType = entity->getEntityType();

    initialiseVectorForEntityTypeStoreIfIndexNotExist(entityType);

    this->entityTypeToStore.at(entity->getEntityType())->push_back(entity);

    if (Statement::isStatement(entity)) {
        initialiseVectorForEntityTypeStoreIfIndexNotExist(Statement::getEntityTypeStatic());
        this->entityTypeToStore.at(Statement::getEntityTypeStatic())->push_back(entity);
    }
}

std::vector<Entity *> *EntityManager::getEntitiesByType(EntityType &entityType) {
    this->initialiseVectorForEntityTypeStoreIfIndexNotExist(entityType);
    return this->entityTypeToStore.at(entityType).get();
}

void EntityManager::initialiseVectorForEntityTypeStoreIfIndexNotExist(EntityType &entityType) {
    if (this->entityTypeToStore.find(entityType) == this->entityTypeToStore.end()) {
        this->entityTypeToStore.insert({entityType, std::make_shared<std::vector<Entity *>>()});
    }
}
