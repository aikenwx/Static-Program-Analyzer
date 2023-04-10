#pragma once

#include <vector>
#include <algorithm>
#include <unordered_set>

#include "query_evaluators/string_conversion_helpers.h"

template<typename Table>
auto CombinedKeys(const Table &table1, const Table &table2) -> typename Table::Header {
  auto all_keys = table1.GetHeader();
  for (const auto &key : table2.GetHeader()) {
    if (table1.HasKey(key)) {
      continue;
    }
    all_keys.push_back(key);
  }
  return all_keys;
}

template<typename Table>
auto CommonKeys(const Table &table1, const Table &table2) -> typename Table::Header {
  typename Table::Header common_keys;
  for (const auto &key : table2.GetHeader()) {
    if (table1.HasKey(key)) {
      common_keys.push_back(key);
    }
  }
  return common_keys;
}

template<typename Table>
auto ExtractRow(const Table &table, size_t row_idx, const typename Table::Header &header) -> typename Table::Row {
  typename Table::Row row;
  row.reserve(header.size());
  for (const auto &key : header) {
    row.push_back(table.GetCell(row_idx, key));
  }
  return row;
}

template<typename Table>
auto Project(const Table &table, const typename Table::Header &header) -> Table {
  std::unordered_set<std::string> rows;
  Table projected_table(header);
  for (int row_idx = 0; row_idx < table.ResultSize(); ++row_idx) {
    auto extracted_row = ExtractRow(table, row_idx, header);
    auto hash_key = qps::Join(extracted_row, "/");
    if (rows.find(hash_key) != rows.end()) {
      continue;
    }
    rows.insert(std::move(hash_key));
    projected_table.AddRow(std::move(extracted_row));
  }
  return projected_table;
}
