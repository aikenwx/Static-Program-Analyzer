//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_PKB_H
#define SPA_PKB_H

#include <memory>

#include "CFGStorage.h"
#include "EntityManager.h"
#include "PatternManager.h"
#include "PopulateFacade.h"
#include "QueryFacade.h"
#include "RelationshipManager.h"

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

  // for testing purposes
  PKB(const std::shared_ptr<EntityManager>& entityManager,
      const std::shared_ptr<RelationshipManager>& relationshipManager,
      const std::shared_ptr<PatternManager>& patternManager,
      const std::shared_ptr<CFGStorage>& cfgManager);

  auto getPopulateFacade() -> PopulateFacade*;
  auto getQueryFacade() -> QueryFacade*;
};

#endif  // SPA_PKB_H
