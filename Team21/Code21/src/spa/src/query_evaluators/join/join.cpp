#include "join.h"

#include <algorithm>

namespace qps {
void HashJoin::ConstructHashedTable(const SynonymTable &hash_table) {
  for (size_t row_idx = 0; row_idx < hash_table.NumRows(); row_idx++) {
    hashed_records_[GetHashKey(hash_table, common_synonyms_, row_idx)].push_back(row_idx);
  }
}

std::string HashJoin::GetHashKey(const SynonymTable &table,
                                 const std::vector<Synonym> &common_synoyms,
                                 size_t row_idx) {
  std::string key;
  for (const auto &syn : common_synoyms) {
    key += table.GetCell(syn, row_idx) + "/";
  }
  return key;
}

SynonymTable HashJoin::operator()(const SynonymTable &hash_table, const SynonymTable &probe_table) {
  const auto &hash_table_syns = hash_table.GetSynonyms();
  const auto &probe_table_syns = probe_table.GetSynonyms();
  std::set_intersection(hash_table_syns.begin(), hash_table_syns.end(),
                        probe_table_syns.begin(), probe_table_syns.end(),
                        std::back_inserter(common_synonyms_));
  std::set_union(hash_table_syns.begin(), hash_table_syns.end(),
                 probe_table_syns.begin(), probe_table_syns.end(),
                 std::back_inserter(all_synonyms_));
  std::set_difference(probe_table_syns.begin(), probe_table_syns.end(),
                      common_synonyms_.begin(), common_synonyms_.end(),
                      std::back_inserter(probe_table_unique_syns_));
  ConstructHashedTable(hash_table);

  SynonymTable joined_table(all_synonyms_);
  const auto &hash_table_rows = hash_table.GetResults();
  for (size_t probe_row_idx = 0; probe_row_idx < probe_table.NumRows(); ++probe_row_idx) {
    auto matched_key = hashed_records_.find(GetHashKey(probe_table, common_synonyms_, probe_row_idx));
    if (matched_key == hashed_records_.end()) continue;
    for (auto hashed_table_idx : matched_key->second) {
      SynonymTable::Row row;
      for (const auto &syn : all_synonyms_) {
        if (hash_table.HasSynonym(syn)) {
          row.push_back(hash_table.GetCell(syn, hashed_table_idx));
        } else {
          row.push_back(probe_table.GetCell(syn, probe_row_idx));
        }
      }
      joined_table.AddRow(std::move(row));
    }
  }
  return joined_table;
}
} // qps

