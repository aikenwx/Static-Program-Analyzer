#pragma once

#include <variant>

#include "tables/row_table.h"
#include "query/synonym.h"
#include "query/design_entity.h"
#include "PKB/QueryFacade.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace qps {

using SynonymTable = RowTable<Synonym, Entity *>;
using ClauseResult = std::variant<bool, SynonymTable>;

class ClauseEvaluator {
 public:
  virtual ~ClauseEvaluator() = default;
  virtual ClauseResult Evaluate(QueryFacade &pkb) = 0;
};

} // qps
