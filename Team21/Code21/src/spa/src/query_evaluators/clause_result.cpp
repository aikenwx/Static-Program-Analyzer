#include "clause_result.h"

namespace qps {

ClauseResult::ClauseResult(Synonyms synonyms) {
  for (int i = 0; i < synonyms.size(); ++i) {
    synonym_idx_[synonyms[i].getSynonym()] = i;
  }
}

ClauseResult::SynonymIdx ClauseResult::GetSynonyms() const {
  return synonym_idx_;
}

ClauseResult::Results ClauseResult::GetResults() const {
  return results_;
}

void ClauseResult::AddResult(Result result) {
//  if (result.size() != synonyms_.size()) {
//    throw std::invalid_argument("Size of result tuple doesnt match number of referenced synonyms");
//  }
  results_.push_back(std::move(result));
}

void ClauseResult::AddSynonym(Synonym syn, int index) {
  if (HasSynonym(syn)) {
    throw std::invalid_argument("Synonym already present in result");
  }
  synonym_idx_[syn.getSynonym()] = index;
}

bool ClauseResult::HasSynonym(Synonym &syn) {
  return synonym_idx_.find(syn.getSynonym()) != synonym_idx_.end();
}

ClauseResult::ResultSet ClauseResult::Extract(Synonym synonym) {
  ResultSet result_set;
  if (!HasSynonym(synonym)) {
    throw std::invalid_argument("Extracting synonym not present in result");
  }
  int synonym_idx = synonym_idx_[synonym.getSynonym()];
  for (auto vec : results_) {
    result_set.insert(vec[synonym_idx]);
  }
  return result_set;
}

bool ClauseResult::Empty() {
  return results_.empty();
}
} // qps
