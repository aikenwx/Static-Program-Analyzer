#pragma once

#include <string>

#include <PKB/PKB.h>

namespace sp {
class SP {
 public:
  SP() = default;
  ~SP() = default;
  static auto process(const std::string& program, PKB* pkb) -> bool;
};
}  // namespace sp
