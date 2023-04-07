//
// Created by Aiken Wong on 3/2/23.
//

#include "EntityManager.h"

EntityManager::EntityManager() {
  this->fastAccessStmts = std::vector<Statement *>(
      EntityManager::MAX_NUMBER_OF_STMTS_WITH_BUFFER, nullptr);

    placeholderEntityVector = std::make_unique<std::vector<Entity*>>();
}

void EntityManager::storeEntity(std::unique_ptr<Entity> entity) {
  auto *entityPtr = entity.get();

  if (Statement::isStatement(entity.get())) {
    auto *stmt = dynamic_cast<Statement *>(entity.get());

    if (this->fastAccessStmts.at(stmt->getStatementNumber()) != nullptr) {
        return ;
    }

      this->fastAccessStmts[stmt->getStatementNumber()] = stmt;
      numberOfStatements++;

  }

  else if (!this->entityStore.try_emplace(entity->getEntityKey(), entityPtr)
           .second) {
    // early return if entity already exists
    return;
  }
  entitiesOwner.push_back(std::move(entity));

  this->storeInEntityTypeStore(entityPtr);
}

auto EntityManager::getEntity(EntityKey &key) -> Entity * {
  if (StatementType::isStatementType(*key.entityType) &&
      key.getOptionalInt() != nullptr) {
    return fastAccessStmts.at(*key.getOptionalInt());
  }

  if (entityStore.find(key) != entityStore.end()) {
    return this->entityStore.at(key);
  }

  return nullptr;
}

void EntityManager::storeInEntityTypeStore(Entity *entity) {
  const EntityType &entityType = entity->getEntityType();
    getVectorForEntityTypeStore(entityType)->push_back(entity);

  if (Statement::isStatement(entity)) {
      getVectorForEntityTypeStore( Statement::getEntityTypeStatic())->push_back(entity);
  }
}

auto EntityManager::getEntitiesByType(const EntityType &entityType)
    -> std::vector<Entity *> * {
  if (this->entityTypeToEntityStore.find(entityType) ==
      this->entityTypeToEntityStore.end()) {
    return &this->emptyEntityVector;
  }
  return this->entityTypeToEntityStore.at(entityType);
}

auto EntityManager::getVectorForEntityTypeStore(
    const EntityType &entityType) -> std::vector<Entity *>* {

 // we use fallbackEntityVector so we do not instantiate entityvectors unnecessarily
  auto * placeholderEntityVectorPtr = placeholderEntityVector.get();

  auto emplaceResult = this->entityTypeToEntityStore.try_emplace(
          entityType, placeholderEntityVectorPtr);
   if (emplaceResult.second) {

       entityVectorOwner.push_back(std::move(placeholderEntityVector));

       // reinitialise fallback vector for next insertion
       placeholderEntityVector = std::make_unique<std::vector<Entity *>>();

       return placeholderEntityVectorPtr;
   }
    return  emplaceResult.first->second;
}

auto EntityManager::getNumberOfStatements() const -> int {
  return this->numberOfStatements;
}

auto EntityManager::getStmtByNumber(int stmtNumber) -> Statement * {
  return fastAccessStmts.at(stmtNumber);
}

