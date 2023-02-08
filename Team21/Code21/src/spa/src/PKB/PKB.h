//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_PKB_H
#define SPA_PKB_H

#include "EntityManager.h"
#include "RelationshipManager.h"
#include "PopulateFacade.h"
#include "QueryFacade.h"


class PKB {

private:
    PopulateFacade *populateFacade;
    QueryFacade *queryFacade;
    EntityManager *entityManager;
    RelationshipManager *relationshipManager;
public:
    PKB();

    ~PKB();

    PopulateFacade* getPopulateFacade();
    QueryFacade* getQueryFacade();

};


#endif //SPA_PKB_H
