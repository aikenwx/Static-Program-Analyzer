#include "Constant.h"

Constant::Constant(int constantValue) {
    this->constantValue = constantValue;
    this->constantValueString = std::make_shared<std::string>(std::to_string(constantValue));
}

std::string * Constant::getEntityValue() {
    return constantValueString.get();
}

EntityType Constant::getEntityType() {
    return EntityType::CONSTANT;
}

int Constant::getConstantNumber() {
    return this->constantValue;
}