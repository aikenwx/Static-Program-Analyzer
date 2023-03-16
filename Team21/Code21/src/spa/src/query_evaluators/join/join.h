#pragma once

#include <set>
#include <unordered_map>

#include "query_evaluators/tables/table_helpers.h"
#include "query_evaluators/string_helpers.h"

namespace std {
template<typename T1, typename T2>
struct hash<pair<T1, T2>> {
  auto operator()(const std::pair<T1, T2> &pair) -> size_t {
    return hash<T1>{}(pair.first) ^ hash<T2>{}(pair.second);
  }
};
}

namespace qps {
template<typename Table>
class HashJoin {
 public:
  using JoinHashTable = std::unordered_map<std::string, std::vector<std::size_t>>;

  HashJoin() = default;

  auto operator()(const Table &hash_table, const Table &probe_table) -> Table {
    const auto common_keys = CommonKeys(hash_table, probe_table);
    const auto combined_keys = CombinedKeys(hash_table, probe_table);
    const auto hashed_records = ConstructHashedTable(hash_table, common_keys);

    Table joined_table(combined_keys);
    const auto &hash_table_rows = hash_table.GetRows();
    for (std::size_t probe_row_idx = 0; probe_row_idx < probe_table.ResultSize(); ++probe_row_idx) {
      auto matched_key = hashed_records.find(GetHashKey(probe_table, common_keys, probe_row_idx));
      if (matched_key == hashed_records.end()) {
        continue;
      }
      for (auto hashed_row_idx : matched_key->second) {
        typename Table::Row row;
        for (const auto &key : combined_keys) {
          if (hash_table.HasKey(key)) {
            row.push_back(hash_table.GetCell(hashed_row_idx, key));
          } else {
            row.push_back(probe_table.GetCell(probe_row_idx, key));
          }
        }
        joined_table.AddRow(std::move(row));
      }
    }

    return joined_table;
  }
 private:
  auto ConstructHashedTable(const Table &hash_table, const typename Table::Header &common_keys) -> JoinHashTable {
    JoinHashTable hashed_records;
    for (std::size_t row_idx = 0; row_idx < hash_table.ResultSize(); row_idx++) {
      hashed_records[GetHashKey(hash_table, common_keys, row_idx)].push_back(row_idx);
    }
    return hashed_records;
  }

  auto GetHashKey(const Table &table, const typename Table::Header &common_keys, std::size_t row_idx) -> std::string {
    typename Table::Row extracted_row = ExtractRow(table, row_idx, common_keys);
    return Join(extracted_row, "/");
  }
};
}  // namespace qps
