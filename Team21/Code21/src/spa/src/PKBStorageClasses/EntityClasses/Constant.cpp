#include "Constant.h"

Constant::Constant(int constantValue) {
    this->constantValue = constantValue;
}

std::string Constant::getEntityValue() {
    return std::to_string(this->constantValue);
}

bool Constant::equals(Entity *otherEntity) {
    if (dynamic_cast<Constant *>(otherEntity) != nullptr) {
        return dynamic_cast<Constant *>(otherEntity)->constantValue == this->constantValue;
    }
    return false;
}