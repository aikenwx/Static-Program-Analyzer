#pragma once

#include <optional>
#include <map>
#include <utility>

namespace util {
template <typename Key, typename Val>
class IntervalTree {
  /**
   * A interval tree with intervals of type Key and values of type Val.
   * The intervals are inclusive, and should not overlap.
   */
 public:
  IntervalTree();
  void Insert(std::pair<Key, Key> interval, Val value);
  void Delete(std::pair<Key, Key> interval);
  std::optional<Val> Search(Key value);

 private:
  std::map<std::pair<Key, Key>, Val> intervals_;
};
}  // namespace util
