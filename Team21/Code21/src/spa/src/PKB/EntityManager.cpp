//
// Created by Aiken Wong on 3/2/23.
//

#include "EntityManager.h"

#include <stdexcept>

EntityManager::EntityManager() {
    this->entityTypeToEntityStore = std::unordered_map<EntityType, std::shared_ptr<std::vector<Entity *>>>();
    this->entityStore = std::unordered_map<EntityKey, std::shared_ptr<Entity>>();
}

void EntityManager::storeEntity(Entity *entity) {
    auto sharedEntity = std::shared_ptr<Entity>(entity);
    if (entityStore.find(entity->getEntityKey()) != entityStore.end()) {
        return;
    }
    this->entityStore.insert(std::make_pair(entity->getEntityKey(), sharedEntity));

    if (Statement::isStatement(entity)) {
        EntityKey statementKey = EntityKey(&Statement::getEntityTypeStatic(), entity->getEntityValue());
        this->entityStore.insert(std::make_pair(statementKey, sharedEntity));
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

    this->entityTypeToEntityStore.at(entity->getEntityType())->push_back(entity);

    if (Statement::isStatement(entity)) {
        initialiseVectorForEntityTypeStoreIfIndexNotExist(Statement::getEntityTypeStatic());
        this->entityTypeToEntityStore.at(Statement::getEntityTypeStatic())->push_back(entity);
    }
}

std::vector<Entity *> *EntityManager::getEntitiesByType(EntityType &entityType) {
    this->initialiseVectorForEntityTypeStoreIfIndexNotExist(entityType);
    return this->entityTypeToEntityStore.at(entityType).get();
}

void EntityManager::initialiseVectorForEntityTypeStoreIfIndexNotExist(EntityType &entityType) {
    if (this->entityTypeToEntityStore.find(entityType) == this->entityTypeToEntityStore.end()) {
        this->entityTypeToEntityStore.insert({entityType, std::make_shared<std::vector<Entity *>>()});
    }
}
