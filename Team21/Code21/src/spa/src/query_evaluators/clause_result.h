#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "query/synonym.h"

namespace qps {

class ClauseResult {

  using Synonyms = std::vector<Synonym>;
  using Result = std::vector<std::string>;
  using Results = std::vector<Result>;
  using SynonymIdx = std::unordered_map<std::string, int>;
  using ResultSet = std::unordered_set<std::string>;

 public:
  ClauseResult() {}
  ClauseResult(Synonyms synonyms);
  SynonymIdx GetSynonyms() const;
  Results GetResults() const;
  void AddResult(Result result);
  void AddSynonym(Synonym syn, int index);
  bool HasSynonym(Synonym &syn);
  bool Empty();
  ResultSet Extract(Synonym synonym);

 private:
  Results results_;
  SynonymIdx synonym_idx_;
};
} // qps
