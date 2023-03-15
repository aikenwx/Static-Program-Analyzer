#pragma once

#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace qps {

template<typename K, typename V>
class RowTable {
 public:
  using Key = K;
  using Value = V;
  using Header = std::vector<Key>;
  using HeaderIdx = std::unordered_map<Key, std::size_t>;

  using Row = std::vector<Value>;
  using Rows = std::vector<Row>;

  RowTable(Header header) : header_(std::move(header)) {
    for (std::size_t i = 0; i < header_.size(); ++i) {
      header_idx_[header_[i]] = i;
    }
  }

  RowTable(std::vector<Key> header, Rows results) : header_(std::move(header)), rows_(std::move(results)) {
    for (std::size_t i = 0; i < header_.size(); ++i) {
      header_idx_[header_[i]] = i;
    }
  }

  const Header &GetHeader() const {
    return header_;
  }

  const Rows &GetRows() const {
    return rows_;
  }

  const V &GetCell(std::size_t row, Key key) const {
    return rows_[row][KeyIndex(key)];
  }

  std::size_t KeyIndex(const Key &key) const {
    auto it = header_idx_.find(key);
    if (it != header_idx_.end()) return it->second;
    return -1;
  }

  std::size_t HeaderSize() const {
    return header_.size();
  };

  std::size_t ResultSize() const {
    return rows_.size();
  };

  bool IsEmpty() const {
    return ResultSize() == 0;
  }

  bool HasKey(const Key &key) const {
    return KeyIndex(key) != -1;
  }

  void AddRow(std::vector<Value> row) {
    if (row.size() != header_.size()) throw std::invalid_argument("Row size doesnt match header");
    rows_.push_back(std::move(row));
  }

  Row Extract(const Key &key) {
    if (!HasKey(key)) {
      throw std::invalid_argument("Extracting key not in table");
    }
    Row extracted;
    int key_idx = header_idx_[key];
    for (const auto &row : rows_) {
      extracted.push_back(row[key_idx]);
    }
    return extracted;
  }

  Row ExtractRow(size_t row_idx, const Header &common_keys) {
    Row row;
    row.reserve(common_keys.size());
    for (const auto &key : common_keys) {
      row.push_back(GetCell(row_idx, key));
    }
    return row;
  }

 private:
  Header header_;
  HeaderIdx header_idx_;
  Rows rows_;
};

} // qps
