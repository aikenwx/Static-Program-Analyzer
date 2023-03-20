#pragma once

#include <iostream>

namespace qps {

// Underscore class represents underscore as a type of stmtRef/entRef
class Underscore {

public:
  auto operator==([[maybe_unused]] const Underscore &underscore) const -> bool {
    return true;
  }

  friend auto operator<<(std::ostream &ostream,
                         [[maybe_unused]] Underscore const &underscore)
      -> std::ostream & {
    ostream << "_";
    return ostream;
  }
};
} // namespace qps
