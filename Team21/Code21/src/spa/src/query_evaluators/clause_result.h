#pragma once

#include <vector>
#include <string>

#include "query/declaration.h"

namespace qps {

class ClauseResult {
  using Declarations = std::vector<Declaration>;
  using Result = std::vector<std::string>;
  using Results = std::vector<Result>;
 public:
  ClauseResult(Declarations declarations);
  Declarations GetDeclarations() const;
  Results GetResults() const;
  void AddResult(Result result);

 private:
  Declarations declarations_;
  Results results_;

};

} // qps
