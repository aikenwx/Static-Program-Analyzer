//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_PKB_H
#define SPA_PKB_H

#include <memory>

#include "EntityManager.h"
#include "PopulateFacade.h"
#include "QueryFacade.h"
#include "RelationshipManager.h"
#include "PatternManager.h"
#include "CFGStorage.h"

class PKB {
   private:
    std::shared_ptr<PopulateFacade> populateFacade;
    std::shared_ptr<QueryFacade> queryFacade;
    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<RelationshipManager> relationshipManager;
    std::shared_ptr<PatternManager> patternManager;
    std::shared_ptr<CFGStorage> cfgManager;

   public:
    PKB();

    auto getPopulateFacade() -> PopulateFacade*;
    auto getQueryFacade() -> QueryFacade*;
};

#endif  // SPA_PKB_H
