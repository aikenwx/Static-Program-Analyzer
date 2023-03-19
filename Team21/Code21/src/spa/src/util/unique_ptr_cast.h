#pragma once

#include <memory>

namespace std {
  /**
   * Note: this will cause the original pointer to be zeroed out! Be careful.
  */
  template <typename Derived, typename Base>
  auto static_pointer_cast(std::unique_ptr<Base>&& ptr) -> std::unique_ptr<Derived> {
    return std::unique_ptr<Derived>(static_cast<Derived*>(ptr.release()));
  }
}
