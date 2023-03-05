#pragma once

#include <algorithm>
#include <map>
#include <optional>
#include <utility>

namespace util {
template <typename Key, typename Val>
class IntervalTree {
  /**
   * A interval tree with intervals of type Key and values of type Val.
   * The intervals are inclusive, and (probably) should not overlap.
   */
 public:
  IntervalTree(){};
  bool Insert(std::pair<Key, Key> interval, Val value) {
    if (interval.first > interval.second) {
      return false;
    }
    if (Search(interval.first) != std::nullopt ||
        Search(interval.second) != std::nullopt) {
      return false;
    }
    intervals_.insert({interval, value});
    return true;
  }
  bool Delete(std::pair<Key, Key> interval) {
    return intervals_.erase(interval) > 0;
  };
  std::optional<Val> Search(Key key) {
    auto it = intervals_.lower_bound({value, value});
    if (it == intervals_.end()) {
      return std::nullopt;
    }
    if (it->first.first <= value && it->first.second >= value) {
      return it->second;
    }
    return std::nullopt;
  };

 private:
  std::map<std::pair<Key, Key>, Val> intervals_;
};
}  // namespace util
