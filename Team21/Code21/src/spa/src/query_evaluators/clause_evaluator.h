#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-special-member-functions"
#pragma once

#include <variant>

#include "tables/row_table.h"
#include "query/synonym.h"
#include "query/design_entity.h"
#include "pkb_result.h"
#include "PKB/QueryFacade.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace qps {

using SynonymTable = RowTable<Synonym, Entity *>;
using ClauseResult = std::variant<bool, SynonymTable>;

class ClauseEvaluator {
 public:
  virtual ~ClauseEvaluator() = default;
  virtual auto Evaluate(QueryFacade &pkb) -> ClauseResult = 0;
};

}  // namespace qps
