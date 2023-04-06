#pragma once

#include "query/with_ref.h"

#include <optional>
#include <variant>

namespace qps {

class WithClause {
 private:
  WithRef ref1;
  WithRef ref2;

 public:
  WithClause(WithRef ref1_, WithRef ref2_);
  auto getRef1() const -> const WithRef &;
  auto getRef2() const -> const WithRef &;

  auto operator==(const WithClause &clause) const -> bool {
    return ref1 == clause.ref1 && ref2 == clause.ref2;
  }
};

}  // namespace qps
