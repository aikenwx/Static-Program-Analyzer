//
// Created by Aiken Wong on 9/3/23.
//

#include "StorageKey.h"

StorageKey::StorageKey(std::size_t key) : key(key) {

}

auto StorageKey::getKey() const -> const std::size_t & {
    return this->key;
}

auto std::hash<StorageKey>::operator()(const StorageKey &storageKey) const -> size_t {
    return storageKey.getKey();
}

auto StorageKey::operator==(const StorageKey &storageKey) const -> bool {
    return this->key == storageKey.key;
}
