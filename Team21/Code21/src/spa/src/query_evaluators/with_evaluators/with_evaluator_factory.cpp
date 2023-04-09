#include "with_evaluator_factory.h"
#include "./query/declaration.h"
#include "query/attr_ref.h"
#include "query/quoted_identifier.h"
#include "query/synonym.h"
#include "query/with_ref.h"
#include "query_evaluators/with_evaluators/with_double_attr_evaluator.h"
#include "query_evaluators/with_evaluators/with_double_int_evaluator.h"
#include "query_evaluators/with_evaluators/with_double_quote_evaluator.h"
#include "query_evaluators/with_evaluators/with_int_attr_evaluator.h"
#include "query_evaluators/with_evaluators/with_quote_attr_evaluator.h"

namespace qps {
auto WithEvaluatorFactory::Create(const WithClause &clause,
                                  const std::vector<Declaration> &decl_lst) -> std::unique_ptr<WithEvaluator> {
  auto ref1 = clause.getRef1();
  auto ref2 = clause.getRef2();
  auto *leftQuoted = std::get_if<QuotedIdentifier>(&ref1.ref);
  auto *rightQuoted = std::get_if<QuotedIdentifier>(&ref2.ref);
  auto *leftInt = std::get_if<int>(&ref1.ref);
  auto *rightInt = std::get_if<int>(&ref2.ref);
  auto *leftAttrRef = std::get_if<AttrRef>(&ref1.ref);
  auto *rightAttrRef = std::get_if<AttrRef>(&ref2.ref);

  //QuotedIdentifier, int, AttrRef
  if (leftQuoted != nullptr && (rightQuoted != nullptr)) {
    return std::make_unique<WithDoubleQuoteEvaluator>(clause, decl_lst);
  }
  if (leftInt != nullptr && (rightInt != nullptr)) {
    return std::make_unique<WithDoubleIntEvaluator>(clause, decl_lst);
  }
  if (leftAttrRef != nullptr && (rightAttrRef != nullptr)) {
    return std::make_unique<WithDoubleAttrEvaluator>(clause, decl_lst);
  }
  if (leftInt != nullptr && (rightAttrRef != nullptr)) {
    return std::make_unique<WithIntAttrEvaluator>(clause, decl_lst);
  }
  if (leftQuoted != nullptr && (rightAttrRef != nullptr)) {
    return std::make_unique<WithQuoteAttrEvaluator>(clause, decl_lst);
  }
  if (leftAttrRef != nullptr && (rightInt != nullptr)) {
    return std::make_unique<WithIntAttrEvaluator>(clause, decl_lst);
  }
  if (leftAttrRef != nullptr && (rightQuoted != nullptr)) {
    return std::make_unique<WithQuoteAttrEvaluator>(clause, decl_lst);
  }
  return nullptr;
}
}  // namespace qps
