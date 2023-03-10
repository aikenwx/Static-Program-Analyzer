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
    StorageKey(std::size_t key);
    const std::size_t &getKey() const;

    bool operator==(const StorageKey &storageKey) const;
};

template <>
struct std::hash<StorageKey> {
    std::size_t operator()(const StorageKey &storageKey) const;
};

#endif  // SPA_STORAGEKEY_H
