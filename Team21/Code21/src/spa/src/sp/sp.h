#pragma once

#include <string>

#include <PKB/PKB.h>

namespace sp {
class SP {
 public:
  SP() = delete;
  static auto process(const std::string& program, PKB* pkb) -> bool;
};
}  // namespace sp
