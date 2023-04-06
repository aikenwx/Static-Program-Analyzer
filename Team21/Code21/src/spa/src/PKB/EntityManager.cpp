//
// Created by Aiken Wong on 3/2/23.
//

#include "EntityManager.h"

EntityManager::EntityManager() {
    this->fastAccessStmts = std::vector<Statement *>(EntityManager::MAX_NUMBER_OF_STMTS_WITH_BUFFER, nullptr);

    this->entityTypeToEntityStore =
      std::unordered_map<EntityType, std::shared_ptr<std::vector<Entity *>>>();
  this->entityStore = std::unordered_map<EntityKey, std::shared_ptr<Entity>>();
}

void EntityManager::storeEntity(const std::shared_ptr<Entity> &entity) {
  if (!this->entityStore.try_emplace(entity->getEntityKey(), entity).second) {
    // early return if entity already exists
    return;
  }

  if (Statement::isStatement(entity.get())) {
    EntityKey statementKey =
        EntityKey(&Statement::getEntityTypeStatic(), entity->getEntityValue());
    this->entityStore.try_emplace(statementKey, entity);

    auto* stmt = dynamic_cast<Statement*>(entity.get());
    this->fastAccessStmts[stmt->getStatementNumber()] = stmt;
    numberOfStatements++;
  }

  this->storeInEntityTypeStore(entity.get());
}

auto EntityManager::getEntity(EntityKey &key) -> Entity * {
    if (StatementType::isStatementType(*key.entityType) && key.getOptionalInt() != nullptr) {
        return fastAccessStmts.at(*key.getOptionalInt());
    }

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
    initialiseVectorForEntityTypeStoreIfIndexNotExist(
        Statement::getEntityTypeStatic());
    this->entityTypeToEntityStore.at(Statement::getEntityTypeStatic())
        ->push_back(entity);
  }
}

auto EntityManager::getEntitiesByType(const EntityType &entityType)
    -> std::vector<Entity *> * {
  if (this->entityTypeToEntityStore.find(entityType) ==
      this->entityTypeToEntityStore.end()) {
    return &this->emptyEntityVector;
  }
  return this->entityTypeToEntityStore.at(entityType).get();
}

void EntityManager::initialiseVectorForEntityTypeStoreIfIndexNotExist(
    const EntityType &entityType) {
  // below line only inserts if the key doesn't already exist
  this->entityTypeToEntityStore.try_emplace(
      entityType, std::make_shared<std::vector<Entity *>>());
}

auto EntityManager::getNumberOfStatements() const -> int {
  return this->numberOfStatements;
}

auto EntityManager::getStmtByNumber(int stmtNumber) -> Statement * {
    return fastAccessStmts.at(stmtNumber);
}
