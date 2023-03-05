#include "interval_tree.h"

#include <algorithm>

namespace util {
template <typename Key, typename Val>
IntervalTree<Key, Val>::IntervalTree() {}

template <typename Key, typename Val>
void IntervalTree<Key, Val>::Insert(std::pair<Key, Key> interval, Val value) {
  assert(interval.first <= interval.second);
  assert(intervals_.find(interval) == intervals_.end());
  intervals_.insert({interval, value});
}

template <typename Key, typename Val>
void IntervalTree<Key, Val>::Delete(std::pair<Key, Key> interval) {
  intervals_.erase(interval);
}

template <typename Key, typename Val>
std::optional<Val> IntervalTree<Key, Val>::Search(Key value) {
  auto it = intervals_.lower_bound({value, value});
  if (it == intervals_.end()) {
    return std::nullopt;
  }
  if (it->first.first <= value && it->first.second >= value) {
    return it->second;
  }
  return std::nullopt;
}
}  // namespace util
