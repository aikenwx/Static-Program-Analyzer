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
  using Rows [[maybe_unused]] = std::vector<Row>;

  RowTable() = default;

  [[maybe_unused]]
  explicit RowTable(Header header) : header_(std::move(header)) {
    for (std::size_t i = 0; i < header_.size(); ++i) {
      header_idx_[header_[i]] = i;
    }
  }

  [[maybe_unused]]
  RowTable(std::vector<Key> header, Rows results)
      : header_(std::move(header)), rows_(std::move(results)) {
    for (std::size_t i = 0; i < header_.size(); ++i) {
      header_idx_[header_[i]] = i;
    }
  }

  [[maybe_unused]]
  RowTable(Key key, Row col)
      : header_({key}) {
    header_idx_[key] = 0;
    for (auto &val : col) {
      rows_.push_back({std::move(val)});
    }
  }

  auto GetHeader() const -> const Header & {
    return header_;
  }

  auto GetRows() const -> const Rows & {
    return rows_;
  }

  auto GetCell(std::size_t row, Key key) const -> const V & {
    return rows_[row][KeyIndex(key)];
  }

  auto KeyIndex(const Key &key) const -> std::size_t {
    auto iterator = header_idx_.find(key);
    if (iterator != header_idx_.end()) {
      return iterator->second;
    }
    return -1;
  }

  [[nodiscard]] auto HeaderSize() const -> std::size_t {
    return header_.size();
  };

  [[nodiscard]] auto ResultSize() const -> std::size_t {
    return rows_.size();
  };

  [[nodiscard]] auto IsEmpty() const -> bool {
    return ResultSize() == 0;
  }

  explicit operator bool() const {
    return !IsEmpty();
  }

  auto HasKey(const Key &key) const -> bool {
    return KeyIndex(key) != -1;
  }

  void AddRow(std::vector<Value> row) {
    if (row.size() != header_.size()) {
      throw std::invalid_argument("Row size doesnt match header");
    }
    rows_.push_back(std::move(row));
  }

  [[maybe_unused]] auto Extract(const Key &key) -> Row {
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

  [[maybe_unused]] auto ExtractRow(size_t row_idx, const Header &common_keys) -> Row {
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

}  // namespace qps
