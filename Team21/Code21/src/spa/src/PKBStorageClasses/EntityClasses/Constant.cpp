#include "Constant.h"

Constant::Constant(int constantValue) : Entity(&Constant::getEntityType(), std::make_shared<std::string>(std::to_string(constantValue))) {
    this->constantValue = constantValue;
}

EntityType &Constant::getEntityType() const {
    return this->entityType;
}

int Constant::getConstantNumber() const {
    return this->constantValue;
}

EntityType &Constant::getEntityTypeStatic() {
    return Constant::entityType;
}

EntityType Constant::entityType = EntityType();
