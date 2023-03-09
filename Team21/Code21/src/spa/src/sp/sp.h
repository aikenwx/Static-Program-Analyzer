#pragma once

#include <string>

#include <PKB/PKB.h>

namespace sp {
class SP {
 public:
  SP() = default;
  ~SP() = default;
  bool process(const std::string& program, PKB* pkb) const;
};
}  // namespace sp
