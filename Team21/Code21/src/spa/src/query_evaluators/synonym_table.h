#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <set>
#include <map>

#include "query/synonym.h"

namespace std {
template<>
struct hash<qps::Synonym> {
  size_t operator()(const qps::Synonym &syn) const {
    return hash<std::string>{}(syn.getSynonym());
  }
};
}

namespace qps {
class SynonymTable {
 public:
  using SynonymIdx = std::map<Synonym, size_t>;
  using Value = std::string;
  using Row = std::vector<Value>;
  using ResultSet = std::unordered_set<Value>;

  SynonymTable() = default;
  SynonymTable(const std::vector<Synonym> &synonyms);
  SynonymTable(const std::vector<Synonym> &synonyms, const std::vector<Row> &rows);

  const std::set<Synonym> &GetSynonyms() const;
  const std::vector<Row> &GetResults() const;
  const Value &GetCell(const Synonym &syn, size_t row_idx) const;
  bool HasSynonym(const Synonym &synonym) const;
  bool Empty() const;
  size_t NumRows() const;
  void AddRow(Row row);

  ResultSet Extract(const Synonym &synonym);

 private:
  std::vector<Row> rows_;
  SynonymIdx synonym_idx_;
  std::set<Synonym> syns_;
};
} // qps
