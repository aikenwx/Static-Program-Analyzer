//
// Created by Aiken Wong on 2/2/23.
//

#include "Variable.h"

auto Variable::getEntityTypeStatic() -> const EntityType & {
    return Variable::variableType;
}

const EntityType Variable::variableType = EntityType();

Variable::Variable(const std::shared_ptr<std::string>& variableValue) : Entity(&Variable::getEntityTypeStatic(), variableValue) {
}

auto Variable::getEntityType() const -> const EntityType & {
    return Variable::getEntityTypeStatic();
}
