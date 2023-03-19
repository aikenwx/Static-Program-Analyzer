#include "Constant.h"

Constant::Constant(int constantValue)
    : Entity(&Constant::getEntityType(),
             std::make_shared<std::string>(std::to_string(constantValue)))
             {}

auto Constant::getEntityType() const -> const EntityType & {
  return Constant::entityType;
}

auto Constant::getEntityTypeStatic() -> const EntityType & {
  return Constant::entityType;
}

const EntityType Constant::entityType = EntityType();
