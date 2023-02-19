#include "synonym_table.h"

namespace qps {

SynonymTable::SynonymTable(const std::vector<Synonym> &synonyms) {
  for (int i = 0; i < synonyms.size(); ++i) {
    synonym_idx_[synonyms[i]] = i;
    syns_.insert(synonyms[i]);
  }
}

SynonymTable::SynonymTable(const std::vector<Synonym> &synonyms, const std::vector<Row> &rows)
    : SynonymTable(synonyms) {
  for (const auto &row : rows) {
    AddRow(row);
  }
};

const std::set<Synonym> &SynonymTable::GetSynonyms() const {
  return syns_;
}

const std::vector<SynonymTable::Row> &SynonymTable::GetResults() const {
  return rows_;
}

void SynonymTable::AddRow(Row row) {
  if (row.size() != synonym_idx_.size()) {
    throw std::invalid_argument("Row does not match synonym header size");
  }
  rows_.push_back(row);
}

const SynonymTable::Value &SynonymTable::GetCell(const Synonym &syn, size_t row_idx) const {
  if (!HasSynonym(syn)) {
    throw std::invalid_argument("Synonym not present in table");
  }

  size_t col_idx = synonym_idx_.find(syn)->second;
  return rows_[row_idx][col_idx];
}

bool SynonymTable::HasSynonym(const Synonym &synonym) const {
  return synonym_idx_.find(synonym.getSynonym()) != synonym_idx_.end();
}

SynonymTable::ResultSet SynonymTable::Extract(const Synonym &synonym) {
  ResultSet result_set;
  if (!HasSynonym(synonym)) {
    throw std::invalid_argument("Extracting synonym not present in result");
  }
  int synonym_idx = synonym_idx_[synonym.getSynonym()];
  for (auto vec : rows_) {
    result_set.insert(vec[synonym_idx]);
  }
  return result_set;
}

bool SynonymTable::Empty() const {
  return rows_.empty();
}

size_t SynonymTable::NumRows() const {
  return rows_.size();
}
} // qps
