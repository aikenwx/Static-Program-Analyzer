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
  IntervalTree() = default;
  bool Insert(std::pair<Key, Key> interval, Val value) {
    if (interval.first > interval.second) {
      return false;
    }
    if (HasOverlap(interval)) {
      return false;
    }
    intervals_.insert({interval, value});
    return true;
  }
  bool Delete(std::pair<Key, Key> interval) {
    return intervals_.erase(interval) > 0;
  };
  std::optional<Val> Search(Key key) const {
    auto closest = SearchClosest(key);
    if (closest == std::nullopt) {
      return std::nullopt;
    }
    if (closest->first.first <= key && closest->first.second >= key) {
      return closest->second;
    }
    return std::nullopt;
  };

 private:
  std::map<std::pair<Key, Key>, Val> intervals_;
  std::optional<std::pair<std::pair<Key, Key>, Val>> SearchClosest(Key key) const {
    const auto& it = std::lower_bound(intervals_.begin(), intervals_.end(), key,
                               [](const auto& kvp1, const auto& k) {
                                 return kvp1.first.second < k;
                               });
    if (it == intervals_.end()) {
      return std::nullopt;
    }
    return *it;
  };
  bool HasOverlap(std::pair<Key, Key> interval1) const {
    std::optional<std::pair<std::pair<Key, Key>, Val>> closest = SearchClosest(interval1.first);
    if (closest == std::nullopt) {
      return false;
    }
    if (Overlaps(interval1, closest->first)) {
      return true;
    }
    closest = SearchClosest(interval1.second);
    if (closest == std::nullopt) {
      return false;
    }
    if (Overlaps(interval1, closest->first)) {
      return true;
    }
    return false;
  };
  static bool Overlaps(std::pair<Key, Key> interval1, std::pair<Key, Key> interval2) {
    // interval1 starts somewhere within interval2
    if (interval1.first >= interval2.first && interval1.first <= interval2.second) {
      return true;
    }
    // interval1 ends somewhere within interval2
    if (interval1.second >= interval2.first && interval1.second <= interval2.second) {
      return true;
    }
    // interval2 starts somewhere within interval1
    if (interval2.first >= interval1.first && interval2.first <= interval1.second) {
      return true;
    }
    // interval2 ends somewhere within interval1
    if (interval2.second >= interval1.first && interval2.second <= interval1.second) {
      return true;
    }
    return false;
  };
};
}  // namespace util
