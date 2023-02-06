#pragma once

#include "clause_result.h"
#include "query/design_entity.h"
#include "PKB/QueryFacade.h"

namespace qps {

class ClauseEvaluator {
 public:
  static EntityType DesignEntityToEntityType(DesignEntity entity);
  virtual ~ClauseEvaluator() = default;
  virtual ClauseResult Evaluate(QueryFacade &pkb) = 0;
};

} // qps
