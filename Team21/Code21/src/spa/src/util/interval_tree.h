#pragma once

#include <algorithm>
#include <map>
#include <optional>
#include <utility>
#include <vector>

namespace util {
template <typename Key, typename Val>
class IntervalTree {
  /**
   * A interval tree with intervals of type Key and values of type Val.
   * The intervals are inclusive, and (probably) should not overlap.
   */
 public:
  IntervalTree() = default;

  auto Insert(std::pair<Key, Key> interval, Val value) -> bool {
    return InsertRetPtr(interval, value).first;
  }

  auto Delete(std::pair<Key, Key> interval) -> bool {
    return intervals_.erase(interval) > 0;
  };

  auto Search(Key key) const -> std::optional<Val> {
    auto closest = SearchClosest(key);
    if (closest == std::nullopt) {
      return std::nullopt;
    }
    if (closest->first.first <= key && closest->first.second >= key) {
      return closest->second;
    }
    return std::nullopt;
  };

  [[nodiscard]] auto Size() const -> int {
    return intervals_.size();
  }

 protected:
  auto InsertRetPtr(std::pair<Key, Key> interval, Val value) -> std::pair<bool, Val*> {
    if (interval.first > interval.second) {
      return std::make_pair(false, nullptr);
    }
    if (HasOverlap(interval)) {
      return std::make_pair(false, nullptr);
    }
    // pair: (iterator pointing to newly inserted (k, v) elem, bool indicating success)
    auto ret = intervals_.insert({interval, value});
    return std::make_pair(true, &ret.first->second);
  }

 private:
  std::map<std::pair<Key, Key>, Val> intervals_;

  auto SearchClosest(Key key) const -> std::optional<std::pair<std::pair<Key, Key>, Val>> {
    const auto& iter = std::lower_bound(intervals_.begin(), intervals_.end(), key,
                               [](const auto& kvp1, const auto& curKey) {
                                 return kvp1.first.second < curKey;
                               });
    if (iter == intervals_.end()) {
      return std::nullopt;
    }
    return *iter;
  };

  auto HasOverlap(std::pair<Key, Key> interval1) const -> bool {
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

  static auto Overlaps(std::pair<Key, Key> interval1, std::pair<Key, Key> interval2) -> bool {
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
