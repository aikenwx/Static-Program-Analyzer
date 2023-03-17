//
// Created by Aiken Wong on 3/2/23.
//

#include "EntityManager.h"

EntityManager::EntityManager() {
    this->entityTypeToEntityStore = std::unordered_map<EntityType, std::shared_ptr<std::vector<Entity *>>>();
    this->entityStore = std::unordered_map<EntityKey, std::shared_ptr<Entity>>();
}

void EntityManager::storeEntity(const std::shared_ptr<Entity>& entity) {
    if (entityStore.find(entity->getEntityKey()) != entityStore.end()) {
        return;
    }
    this->entityStore.try_emplace(entity->getEntityKey(), entity);

    if (Statement::isStatement(entity.get())) {
        EntityKey statementKey = EntityKey(&Statement::getEntityTypeStatic(), entity->getEntityValue());
        this->entityStore.try_emplace(statementKey, entity);
    }

    this->storeInEntityTypeStore(entity.get());
}

auto EntityManager::getEntity(EntityKey &key) -> Entity * {
  if (entityStore.find(key) != entityStore.end()) {
    return this->entityStore.at(key).get();
  }

  return nullptr;
}

void EntityManager::storeInEntityTypeStore(Entity *entity) {
    const EntityType &entityType = entity->getEntityType();

    initialiseVectorForEntityTypeStoreIfIndexNotExist(entityType);

    this->entityTypeToEntityStore.at(entity->getEntityType())->push_back(entity);

    if (Statement::isStatement(entity)) {
        initialiseVectorForEntityTypeStoreIfIndexNotExist(Statement::getEntityTypeStatic());
        this->entityTypeToEntityStore.at(Statement::getEntityTypeStatic())->push_back(entity);
    }
}

auto EntityManager::getEntitiesByType(const EntityType &entityType)
    -> std::vector<Entity *> * {
  this->initialiseVectorForEntityTypeStoreIfIndexNotExist(entityType);
  return this->entityTypeToEntityStore.at(entityType).get();
}

void EntityManager::initialiseVectorForEntityTypeStoreIfIndexNotExist(const EntityType &entityType) {
    // below line only inserts if the key doesn't already exist
    this->entityTypeToEntityStore.try_emplace(entityType, std::make_shared<std::vector<Entity *>>());
}
