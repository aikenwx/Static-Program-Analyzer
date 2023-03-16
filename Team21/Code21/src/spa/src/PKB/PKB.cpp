//
// Created by Aiken Wong on 4/2/23.
//

#include "PKB.h"

PKB::PKB() {
    this->entityManager = std::make_shared<EntityManager>();
    this->relationshipManager = std::make_shared<RelationshipManager>();
    this->patternManager = std::make_shared<PatternManager>();
    this->cfgManager = std::make_shared<CFGManager>();
    this->populateFacade = std::make_shared<PopulateFacade>(entityManager.get(), relationshipManager.get(), patternManager.get(), cfgManager.get());
    this->queryFacade = std::make_shared<QueryFacade>(entityManager.get(), relationshipManager.get(), patternManager.get(), cfgManager.get());
}

auto PKB::getPopulateFacade() -> PopulateFacade* {
  return this->populateFacade.get();
}

auto PKB::getQueryFacade() -> QueryFacade* { return this->queryFacade.get(); }
