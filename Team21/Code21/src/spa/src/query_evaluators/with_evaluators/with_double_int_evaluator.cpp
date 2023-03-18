#include "with_double_int_evaluator.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "query/query_exceptions.h"
#include <unordered_set>
#include <vector>

namespace qps {
auto WithDoubleIntEvaluator::CallPkb(QueryFacade &pkb) -> std::vector<std::vector<Entity *>*> {
  std::vector<std::vector<Entity *>*> equalResult;
  std::vector<Entity *> trueResult;
  WithRef ref1 = getClause().getRef1();
  WithRef ref2 = getClause().getRef2();
  int ref1Num = std::get<int>(ref1.ref);
  int ref2Num = std::get<int>(ref2.ref);

  if (ref1Num == ref2Num) {
    equalResult.push_back(&trueResult);
    return equalResult;
  }
  return equalResult;
}
}  // namespace qps
