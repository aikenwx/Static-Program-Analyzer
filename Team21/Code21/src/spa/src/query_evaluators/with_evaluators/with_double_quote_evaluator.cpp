#include "with_double_quote_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "query/query_exceptions.h"
#include "query/quoted_identifier.h"
#include <unordered_set>
#include <vector>

namespace qps {
auto WithDoubleQuoteEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>> {
  std::vector<std::vector<Entity *>> equalResult = {};
  std::vector<Entity *> trueResult = {};
  WithRef ref1 = getClause().getRef1();
  WithRef ref2 = getClause().getRef2();
  QuotedIdentifier ref1Quote = std::get<QuotedIdentifier>(ref1.ref);
  QuotedIdentifier ref2Quote = std::get<QuotedIdentifier>(ref2.ref);

  if (ref1Quote.getQuotedId() == ref2Quote.getQuotedId()) {
    equalResult.push_back(trueResult);
    return equalResult;
  }
  return equalResult;
}
}  // namespace qps
