#pragma once

#include <vector>
#include <algorithm>
#include <unordered_set>

template<typename Table>
typename Table::Header CombinedKeys(const Table &table1, const Table &table2) {
  auto all_keys = table1.GetHeader();
  for (const auto &key : table2.GetHeader()) {
    if (table1.HasKey(key)) continue;
    all_keys.push_back(key);
  }
  return all_keys;
}

template<typename Table>
typename Table::Header CommonKeys(const Table &table1, const Table &table2) {
  typename Table::Header common_keys;
  for (const auto &key : table2.GetHeader()) {
    if (table1.HasKey(key)) common_keys.push_back(key);
  }
  return common_keys;
}

template<typename Table>
typename Table::Row ExtractRow(const Table &table, size_t row_idx, const typename Table::Header &header) {
  typename Table::Row row;
  row.reserve(header.size());
  for (const auto &key : header) {
    row.push_back(table.GetCell(row_idx, key));
  }
  return row;
}

template<typename Table, typename Container, typename TransformFunc>
void Extract(const Table &table, const Container &container, TransformFunc func) {
  const auto &rows = table.GetResults();
  std::transform(std::begin(rows), std::end(rows), std::back_inserter(container), func);
}

template<typename Table>
std::unordered_set<typename Table::Value> ExtractCol(const Table &table,
                                                     const typename Table::Key &key) {
  std::unordered_set<typename Table::Value> col_vals;
  for (size_t row_idx = 0; row_idx < table.ResultSize(); ++row_idx) {
    col_vals.insert(table.GetCell(row_idx, key));
  }
  return col_vals;
}

