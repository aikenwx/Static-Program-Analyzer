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


EntityType Variable::getEntityType() {
    return EntityType::VARIABLE;
}

