#pragma once

#include <iostream>

namespace qps {

  // Boolean class represents boolean as a type of Select Clause
  class Boolean {

  public:

    bool operator==(const Boolean& us) const {
      return true;
    }

    friend std::ostream& operator<<(std::ostream& os, Boolean const& b) {
      os << "BOOLEAN";
      return os;
    }
  };
}
