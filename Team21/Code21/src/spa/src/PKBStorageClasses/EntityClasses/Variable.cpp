//
// Created by Aiken Wong on 2/2/23.
//

#include "Variable.h"

auto Variable::getEntityTypeStatic() -> EntityType& {
    return Variable::variableType;
}

EntityType Variable::variableType = EntityType();

Variable::Variable(const std::shared_ptr<std::string>& variableValue) : Entity(&Variable::getEntityTypeStatic(), variableValue) {
}

auto Variable::getEntityType() const -> EntityType& {
    return Variable::getEntityTypeStatic();
}
