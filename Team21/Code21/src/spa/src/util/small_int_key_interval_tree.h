#pragma once

#include "util/interval_tree.h"
#include <algorithm>
#include <map>
#include <optional>
#include <utility>
#include <vector>

namespace util {
template <typename Key, typename Val>
class SmallIntKeyIntervalTree : public IntervalTree<Key, Val> {
  /**
   * Does the same thing as IntervalTree, except that it only works for
   * non-negative int intervals. (It'll probably break otherwise)
   * The values are also stored in a vector for fast lookup.
   */
public:
  SmallIntKeyIntervalTree() = default;

  auto Insert(std::pair<Key, Key> interval, Val value) -> bool {
    if (interval.first < 0) {
      return false;
    }
    std::pair<bool, Val *> result =
        IntervalTree<Key, Val>::InsertRetPtr(interval, value);
    if (result.first) {
      if (vals_.size() <= interval.second) {
        vals_.resize(interval.second + 1);
      }
      for (int i = interval.first; i <= interval.second; i++) {
        vals_[i] = result.second;
      }
    }
    return result.first;
  }

  auto Delete(std::pair<Key, Key> interval) -> bool {
    bool result = IntervalTree<Key, Val>::Delete(interval);
    if (result) {
      for (int i = interval.first; i <= interval.second; i++) {
        vals_[i] = std::nullopt;
      }
    }
    return result;
  };

  auto Search(Key key) const -> std::optional<Val> {
    if (vals_.size() <= key) {
      return std::nullopt;
    }
    if (std::optional<Val *> val = vals_[key]; val.has_value()) {
      return *val.value();
    }

    return std::nullopt;
  };

private:
  std::vector<std::optional<Val *>> vals_;
};
} // namespace util
