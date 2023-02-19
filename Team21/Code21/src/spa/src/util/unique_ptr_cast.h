#pragma once

#include <memory>

namespace std {
  /**
   * Note: this will cause the original pointer to be zeroed out! Be careful.
  */
  template <typename Derived, typename Base>
  std::unique_ptr<Derived> static_pointer_cast(std::unique_ptr<Base>&& p) {
    return std::unique_ptr<Derived>(static_cast<Derived*>(p.release()));
  }
}
