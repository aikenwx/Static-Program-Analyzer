#include "Constant.h"

Constant::Constant(int constantValue) {
    this->constantValue = constantValue;
}

std::string Constant::getEntityValue() {
    return std::to_string(this->constantValue);
}

EntityType Constant::getEntityType() {
    return EntityType::CONSTANT;
}
