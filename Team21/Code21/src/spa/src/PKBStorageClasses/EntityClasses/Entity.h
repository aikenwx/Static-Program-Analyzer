//
// Created by Aiken Wong on 1/2/23.
//

#ifndef SPA_ENTITY_H
#define SPA_ENTITY_H

#include <string>
#include <vector>

#include "../StorageKey.h"

struct EntityType : public StorageKey {
   private:
    static std::size_t entityTypeKeyCounter;

   public:
    EntityType();

    bool operator==(const EntityType &entityType) const;
};

template <>
struct std::hash<EntityType> {
    std::size_t operator()(const EntityType &entityType) const;
};

struct EntityKey : public StorageKey {
   private:
    EntityType *entityType;
    std::string *entityValue;
    std::string entityValueStore;

   public:
    EntityKey(EntityType *entityType, std::string *entityValue);
    EntityKey(EntityType *entityType, int entityIntValue);

    bool operator==(const EntityKey &entityKey) const;
};

class Entity {
   private:
    EntityKey entityKey;
    std::shared_ptr<std::string> entityValue;

   public:
    Entity(EntityType *entityType, std::shared_ptr<std::string> entityValue);

    virtual ~Entity(){};

    virtual EntityType &getEntityType() const = 0;

    std::string *getEntityValue() const;

    bool equals(Entity *otherEntity);

    EntityKey &getEntityKey();
};

template <>
struct std::hash<EntityKey> {
    std::size_t operator()(const EntityKey &entityKey) const;
};

#endif
