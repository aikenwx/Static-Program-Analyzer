//
// Created by Aiken Wong on 8/3/23.
//

#ifndef SPA_STORAGEKEY_H
#define SPA_STORAGEKEY_H

#include <cstdlib>
#include <vector>

class StorageKey {
   private:
    std::size_t key;

   public:
    explicit StorageKey(std::size_t key);
    [[nodiscard]] auto getKey() const -> const std::size_t &;

    auto operator==(const StorageKey &storageKey) const -> bool;
};

template <>
struct std::hash<StorageKey> {
    auto operator()(const StorageKey &storageKey) const -> std::size_t;
};

#endif  // SPA_STORAGEKEY_H
