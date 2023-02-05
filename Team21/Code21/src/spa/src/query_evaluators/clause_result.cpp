#include "clause_result.h"

namespace qps {

ClauseResult::ClauseResult(Declarations declarations): declarations_(std::move(declarations)) {}

ClauseResult::Declarations ClauseResult::GetDeclarations() const {
  return declarations_;
}

ClauseResult::Results ClauseResult::GetResults() const {
  return results_;
}

void ClauseResult::AddResult(Result result) {
  if (result.size() != declarations_.size()) {
    throw std::invalid_argument("Size of result tuple doesnt match number of referenced synonyms");
  }
  results_.push_back(std::move(result));
}

} // qps
