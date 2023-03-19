#pragma once

#include <memory>

namespace util {
  /*
  * Taken from https://www.tutorialspoint.com/cplusplus-equivalent-of-instanceof
  */
  template<typename Base, typename T>
  inline auto instance_of(const T* ptr) -> bool {
    return dynamic_cast<const Base*>(ptr) != nullptr;
  }

  template<typename Base, typename T>
  inline auto instance_of(std::shared_ptr<T> ptr) -> bool {
    return std::dynamic_pointer_cast<const Base>(ptr) != nullptr;
  }

  template<typename Base, typename T>
  inline auto instance_of(const std::unique_ptr<T>& ptr) -> bool {
    return dynamic_cast<const Base*>(ptr.get()) != nullptr;
  }
}
