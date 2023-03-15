//
// Created by Aiken Wong on 9/3/23.
//

#include "StorageKey.h"

StorageKey::StorageKey(std::size_t key) {
    this->key = key;
}

const std::size_t &StorageKey::getKey() const {
    return this->key;
}

size_t std::hash<StorageKey>::operator()(const StorageKey &storageKey) const {
    return storageKey.getKey();
}

bool StorageKey::operator==(const StorageKey &storageKey) const {
    return this->key == storageKey.key;
}
