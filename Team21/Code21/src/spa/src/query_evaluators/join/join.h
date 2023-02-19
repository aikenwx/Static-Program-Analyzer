#pragma once

#include <set>
#include <unordered_map>

#include "query/synonym.h"
#include "query_evaluators/synonym_table.h"

namespace std {
template<typename T1, typename T2>
struct hash<pair<T1, T2>> {
  size_t operator()(const std::pair<T1, T2> &pair) {
    return hash<T1>{}(pair.first) ^ hash<T2>{}(pair.second);
  }
};
}

namespace qps {
using JoinHashTable = std::unordered_map<std::string, std::vector<size_t>>;
class HashJoin {
 public:
  HashJoin() = default;
  SynonymTable operator()(const SynonymTable &hash_table, const SynonymTable &probe_table);
 private:
  std::vector<Synonym> common_synonyms_;
  std::vector<Synonym> all_synonyms_;
  std::vector<Synonym> probe_table_unique_syns_;
  JoinHashTable hashed_records_;

  std::string GetHashKey(const SynonymTable &table, const std::vector<Synonym> &common_synoyms, size_t row_idx);
  void ConstructHashedTable(const SynonymTable &hash_table);

};
} // qps
