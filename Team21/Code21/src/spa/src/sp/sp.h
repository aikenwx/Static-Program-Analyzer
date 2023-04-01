#pragma once

#include <string>

#include <PKB/PKB.h>

namespace sp {
class SP {
 public:
  SP() = delete;
  static auto Process(std::string_view program, PKB* pkb) -> bool;
};
}  // namespace sp
