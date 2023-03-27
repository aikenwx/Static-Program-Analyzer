//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NULLABLEENTITY_H
#define SPA_NULLABLEENTITY_H

#include "Entity.h"

class NullableEntity : public Entity {
 private:
  static const EntityType entityType;
  NullableEntity();

  static NullableEntity const nullableEntity;

 public:



  ~NullableEntity() override = default;

  static auto getNullableEntity() -> const NullableEntity &;

  static auto getEntityTypeStatic() -> const EntityType &;

  [[nodiscard]] auto getEntityType() const -> const EntityType & override;
};

#endif  // SPA_NULLABLEENTITY_H
