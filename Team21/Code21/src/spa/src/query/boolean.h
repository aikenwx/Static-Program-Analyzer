#pragma once

#include <iostream>

namespace qps {

  // Underscore class represents underscore as a type of stmtRef/entRef
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
