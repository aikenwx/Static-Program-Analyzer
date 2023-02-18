#pragma once

#include "synonym_table.h"
#include "query/design_entity.h"
#include "PKB/QueryFacade.h"

namespace qps {

class ClauseEvaluator {
 public:
  using ClauseResult = std::variant<bool, SynonymTable>;

  static EntityType DesignEntityToEntityType(DesignEntity entity);
  virtual ~ClauseEvaluator() = default;
  virtual ClauseResult Evaluate(QueryFacade &pkb) = 0;
};

} // qps
