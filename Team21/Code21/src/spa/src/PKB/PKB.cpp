//
// Created by Aiken Wong on 4/2/23.
//

#include "PKB.h"

PKB::PKB() {
    this->entityManager = new EntityManager();
    this->relationshipManager = new RelationshipManager();
    this->populateFacade = new PopulateFacade(entityManager, relationshipManager);
    this->queryFacade = new QueryFacade(entityManager, relationshipManager);
}

PKB::~PKB() {
    delete this->populateFacade;
    delete this->queryFacade;
    delete this->entityManager;
    delete this->relationshipManager;
}

PopulateFacade *PKB::getPopulateFacade() {
    return this->populateFacade;
}

QueryFacade *PKB::getQueryFacade() {
    return this->queryFacade;
}
