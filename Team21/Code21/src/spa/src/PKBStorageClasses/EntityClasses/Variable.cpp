//
// Created by Aiken Wong on 2/2/23.
//

#include "Variable.h"

EntityType &Variable::getEntityTypeStatic() {
    return Variable::variableType;
}

EntityType Variable::variableType = EntityType();

Variable::Variable(std::string *variableValue) : Entity(&Variable::getEntityTypeStatic(), std::shared_ptr<std::string>(variableValue)) {
}

EntityType &Variable::getEntityType() const {
    return Variable::getEntityTypeStatic();
}
