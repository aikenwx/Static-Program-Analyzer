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

void SynonymTable::AddRow(const Row &row) {
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


//
//// Simple nested loop join operation -> and not very fast implementation of that either
//SynonymTable NestedLoopJoin(const SynonymTable& table1, const SynonymTable& table2) {
//  auto common_synonyms = GetCommonSynonyms(table1, table2);
//  auto is_equal = [&](size_t table1_row_idx, size_t table2_row_idx) {
//    for (const auto& syn: common_synonyms) {
//      if(table1.GetCell(syn, table1_row_idx) != table2.GetCell(syn, table2_row_idx)) {
//        return false;
//      }
//    }
//    return true;
//  };
//
//  std::vector<std::pair<size_t, size_t>> join_row_idxs;
//  for(int table1_row_idx = 0; table1_row_idx < table1.NumRows()) {
//    for(int table2_row_idx = 0; table2_row_idx < table2.NumRows()) {
//      if(is_equal(table1_row_idx, table2_row_idx)) {
//        join_row_idxs.push_back({table1_row_idx, table2_row_idx});
//      }
//    }
//  }
//
//  SynonymTable joined_table = ConstructJoinTable()

//}

} // qps
