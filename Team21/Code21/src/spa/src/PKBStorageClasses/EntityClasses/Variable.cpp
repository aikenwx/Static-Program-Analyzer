//
// Created by Aiken Wong on 2/2/23.
//

#include "Variable.h"

Variable::Variable(std::string variableValue) {
    this->variableName = variableValue;
}

std::string Variable::getEntityValue() {
    return this->variableName;
}

bool Variable::equals(Entity *otherEntity) {
    if (dynamic_cast<Variable *>(otherEntity) != nullptr) {
        return dynamic_cast<Variable *>(otherEntity)->variableName == this->variableName;
    }
    return false;
}
