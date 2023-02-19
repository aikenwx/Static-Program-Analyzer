//
// Created by Aiken Wong on 2/2/23.
//

#include "Variable.h"

Variable::Variable(std::string* variableValue) {
    this->variableName = std::shared_ptr<std::string>(variableValue);
}

std::string * Variable::getEntityValue() {
    return this->variableName.get();
}

EntityType Variable::getEntityType() {
    return EntityType::VARIABLE;
}
