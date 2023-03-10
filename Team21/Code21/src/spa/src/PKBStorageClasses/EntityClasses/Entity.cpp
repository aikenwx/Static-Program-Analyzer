//
// Created by Aiken Wong on 3/2/23.
//

#include "Entity.h"

EntityType::EntityType() : StorageKey(EntityType::entityTypeKeyCounter++){};

std::size_t EntityType::entityTypeKeyCounter = 0;

std::size_t std::hash<EntityType>::operator()(const EntityType &entityType) const {
    return entityType.getKey();
}

EntityKey::EntityKey(EntityType *entityType, std::string *entityValue) : StorageKey(
                                                                             entityType->getKey() ^ std::hash<std::string>()(*entityValue)) {
    this->entityType = entityType;
    this->entityValue = entityValue;
}

EntityKey::EntityKey(EntityType *entityType, int entityIntValue) : StorageKey(
                                                                       entityType->getKey() ^ std::hash<std::string>()(std::to_string(entityIntValue))) {
    this->entityType = entityType;
    this->entityValueStore = std::to_string(entityIntValue);
    this->entityValue = &entityValueStore;
}

bool EntityKey::operator==(const EntityKey &entityKey) const {
    return *this->entityType == *entityKey.entityType &&
           *this->entityValue == *entityKey.entityValue;
}

std::size_t std::hash<EntityKey>::operator()(const EntityKey &entityKey) const {
    return entityKey.getKey();
}

std::string *Entity::getEntityValue() const {
    return this->entityValue.get();
}

Entity::Entity(EntityType *entityType, std::shared_ptr<std::string> entityValue) : entityKey(EntityKey(entityType, entityValue.get())) {
    this->entityValue = std::shared_ptr<std::string>(entityValue);
}

EntityKey &Entity::getEntityKey() {
    return this->entityKey;
}

bool Entity::equals(Entity *otherEntity) {
    return this->entityKey == otherEntity->entityKey;
}

bool EntityType::operator==(const EntityType &entityType) const {
    return this->getKey() == entityType.getKey();
}
