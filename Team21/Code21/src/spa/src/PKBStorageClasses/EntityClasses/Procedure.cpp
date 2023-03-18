#include "Procedure.h"

Procedure::Procedure(const std::shared_ptr<std::string>& procedureName) : Entity(&Procedure::getEntityTypeStatic(), procedureName) {
}

auto Procedure::operator==(const Procedure &procedure) const -> bool {
    return this->getEntityValue() == procedure.getEntityValue();
}

auto Procedure::getEntityTypeStatic() -> const EntityType & {
    return Procedure::procedureType;
}

auto Procedure::getEntityType() const -> const EntityType & {
    return Procedure::getEntityTypeStatic();
}

auto std::hash<Procedure>::operator()(const Procedure &procedure) const
    -> size_t {
    return std::hash<std::string *>()(procedure.getEntityValue());
}

const EntityType Procedure::procedureType = EntityType();
