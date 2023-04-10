//
// Created by Aiken Wong on 4/2/23.
//

#include "PKB.h"

PKB::PKB() {
  this->entityManager = std::make_shared<EntityManager>();
  this->relationshipManager =
      std::make_shared<RelationshipManager>(this->entityManager.get());
  this->populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get());
  this->queryFacade = std::make_shared<QueryFacade>(
      entityManager.get(), relationshipManager.get());
}

PKB::PKB(const std::shared_ptr<EntityManager> &entityManager,
         const std::shared_ptr<RelationshipManager> &relationshipManager)
        : entityManager(entityManager), relationshipManager(relationshipManager) {

  this->populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get());

  this->queryFacade = std::make_shared<QueryFacade>(
      entityManager.get(), relationshipManager.get());
}

auto PKB::getPopulateFacade() -> PopulateFacade* {
  return this->populateFacade.get();
}

auto PKB::getQueryFacade() -> QueryFacade* { return this->queryFacade.get(); }
