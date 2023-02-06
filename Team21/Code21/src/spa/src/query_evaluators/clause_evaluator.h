#pragma once

#include "clause_result.h"
#include "query/design_entity.h"
#include "PKB/QueryFacade.h"

namespace qps {

class ClauseEvaluator {
//  static EntityType design_entity_to_entity_type(DesignEntity entity);

 public:
  static EntityType design_entity_to_entity_type(DesignEntity entity);
  virtual ClauseResult evaluate(QueryFacade &pkb) = 0;
  virtual ~ClauseEvaluator() = default;
};
} // qps

