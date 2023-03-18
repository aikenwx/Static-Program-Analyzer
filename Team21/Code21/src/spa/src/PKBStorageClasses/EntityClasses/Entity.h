//
// Created by Aiken Wong on 1/2/23.
//

#ifndef SPA_ENTITY_H
#define SPA_ENTITY_H

#include <memory>
#include <string>
#include <vector>

#include "../StorageKey.h"

struct EntityType : public StorageKey {
 private:
  // NOLINTNEXTLINE (cppcoreguidelines-avoid-non-const-global-variables)
  static std::size_t entityTypeKeyCounter;

 public:
  EntityType();

  auto operator==(const EntityType &entityType) const -> bool;
};

template <>
struct std::hash<EntityType> {
  auto operator()(const EntityType &entityType) const -> std::size_t;
};

struct EntityKey : public StorageKey {
 private:
  const EntityType *entityType;
  std::string *entityValue;
  std::string entityValueStore;

 public:
  EntityKey(const EntityType *entityType, std::string *entityValue);
  EntityKey(const EntityType *entityType, int entityIntValue);

  auto operator==(const EntityKey &entityKey) const -> bool;
};

class Entity {
 private:
  EntityKey entityKey;
  std::shared_ptr<std::string> entityValue;

 public:
  Entity(const EntityType *entityType,
         const std::shared_ptr<std::string> &entityValue);

  virtual ~Entity() = default;
  ;

  [[nodiscard]] virtual auto getEntityType() const -> const EntityType & = 0;

  [[nodiscard]] auto getEntityValue() const -> std::string *;

  auto equals(Entity *otherEntity) -> bool;

  auto getEntityKey() -> EntityKey &;
};

template <>
struct std::hash<EntityKey> {
  auto operator()(const EntityKey &entityKey) const -> std::size_t;
};

#endif
