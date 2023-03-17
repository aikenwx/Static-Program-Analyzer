#include "Constant.h"

Constant::Constant(int constantValue)
    : Entity(&Constant::getEntityType(),
             std::make_shared<std::string>(std::to_string(constantValue))),
      constantValue(constantValue) {}

auto Constant::getEntityType() const -> EntityType& {
  return Constant::entityType;
}

auto Constant::getConstantNumber() const -> int { return this->constantValue; }

auto Constant::getEntityTypeStatic() -> EntityType& {
  return Constant::entityType;
}

EntityType Constant::entityType = EntityType();
