//
// Created by Aiken Wong on 26/3/23.
//

#include "CFGRelationshipEvaluator.h"

CFGRelationshipEvaluator::CFGRelationshipEvaluator(cfg::CFG *cfg , RelationshipStorage *relationshipStorage, EntityManager* entityManager) : cfg(cfg), relationshipStorage(relationshipStorage), entityManager(entityManager) {}

auto CFGRelationshipEvaluator::getCFG() const -> cfg::CFG * { return cfg; }

auto CFGRelationshipEvaluator::getRelationshipStorage() const -> RelationshipStorage * { return relationshipStorage; }

  void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByEntities(Entity *leftEntity, Entity *rightEntity) {
    if (!Statement::isStatement (leftEntity) && !Statement::isStatement (rightEntity)) {
        return;
    }

    auto leftStatement = static_cast<Statement*>(leftEntity);
    this->evaluateAndCacheRelationshipFromLeftStatement(leftStatement);
  }


  void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByEntityTypes(const EntityType &leftEntityType, const EntityType & rightEntityType) {
    auto leftEntityList = this->entityManager->getEntitiesByType(leftEntityType);
    auto rightEntityList = this->entityManager->getEntitiesByType(rightEntityType);

    // if either list is empty, no need to evaluate, relationships cannot exist
    if (leftEntityList->empty() || rightEntityList->empty()) {
        return;
    }
    // if contents of either list is not a statement, no need to evaluate, as they are not CFG evaluable Relationships
    if (!Statement::isStatement(leftEntityList->at(0)) || !Statement::isStatement(rightEntityList->at(0))) {
        return;
    }

    
    int leftEntityCount = 0;

    for (auto leftEntity : *leftEntityList) {
        if (this->relationshipStorage->getEntitiesForGivenRelationshipTypeAndRightHandEntityType((RelationshipType &)this->getRelationshipType(), leftEntity->getEntityKey(), rightEntityType) == nullptr) {
            leftEntityCount++;
        }
    }

    int rightEntityCount = 0;
    for (auto rightEntity : *rightEntityList) {
        if (this->relationshipStorage->getEntitiesForGivenRelationshipTypeAndLeftHandEntityType((RelationshipType &)this->getRelationshipType(), leftEntityType, rightEntity->getEntityKey()) == nullptr) {
            rightEntityCount++;
        }
    }



    // evaluate from the smaller list, which has likely lower cost evaluation and cache results directly into relationship storage
    if (leftEntityCount < rightEntityCount) {
        for (auto leftEntity : *leftEntityList) {
            auto results = this->evaluateAndCacheRelationshipFromLeftStatement(static_cast<Statement*>(leftEntity));

            for (auto result : *results) {
                this->relationshipStorage->storeInRelationshipDoubleSynonymStore(result);
            }
        }
    } else {
        for (auto rightEntity : *rightEntityList) {
            auto results = this->evaluateAndCacheRelationshipFromRightStatement(static_cast<Statement*>(rightEntity));

            for (auto result : *results) {
                this->relationshipStorage->storeInRelationshipDoubleSynonymStore(result);
            }
        }
    }
  }

  void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByLeftEntityTypeAndRightEntity(const EntityType &leftEntityType, Entity *rightEntity) {
    auto leftEntityList = this->entityManager->getEntitiesByType(leftEntityType);

    // if either list is empty, no need to evaluate, relationships cannot exist
    if (leftEntityList->empty()) {
        return;
    }
    // if entities are not statements, no need to evaluate, as they are not CFG evaluable Relationships
    if (!Statement::isStatement(leftEntityList->at(0)) || !Statement::isStatement(rightEntity)) {
        return;
    }


    this->evaluateAndCacheRelationshipFromRightStatement(static_cast<Statement*>(rightEntity));
  }

  void CFGRelationshipEvaluator::evaluateAndCacheRelationshipsByLeftEntityAndRightEntityType(Entity *leftEntity, const EntityType &rightEntityType) {
    auto rightEntityList = this->entityManager->getEntitiesByType(rightEntityType);

    // if either list is empty, no need to evaluate, relationships cannot exist
    if (rightEntityList->empty()) {
        return;
    }
    // if entities are not statements, no need to evaluate, as they are not CFG evaluable Relationships
    if (!Statement::isStatement(leftEntity) || !Statement::isStatement(rightEntityList->at(0))) {
        return;
    }

    this->evaluateAndCacheRelationshipFromLeftStatement(static_cast<Statement*>(leftEntity));
  }