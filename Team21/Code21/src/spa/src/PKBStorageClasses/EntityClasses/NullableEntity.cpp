//
// Created by Aiken Wong on 25/3/23.
//

#include "NullableEntity.h"

const auto NullableEntity::nullableEntity = NullableEntity();

auto NullableEntity::getNullableEntity() -> const NullableEntity & {
    return NullableEntity::nullableEntity;
}


NullableEntity::NullableEntity()
    : Entity(&NullableEntity::getEntityType(),
             std::shared_ptr<std::string>())
             {}

auto NullableEntity::getEntityTypeStatic() -> const EntityType & {
  return NullableEntity::entityType;
}

const EntityType NullableEntity::entityType = EntityType();
