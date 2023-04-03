#pragma once

#include <iostream>

namespace qps {

// Boolean class represents boolean as a type of Select Clause
class Boolean {
 public:
  auto operator==([[maybe_unused]] const Boolean& other) const -> bool {
    return true;
  }

  friend auto operator<<(std::ostream& ostream, [[maybe_unused]] Boolean const& boolean) -> std::ostream& {
    ostream << "BOOLEAN";
    return ostream;
  }
};

}  // namespace qps
