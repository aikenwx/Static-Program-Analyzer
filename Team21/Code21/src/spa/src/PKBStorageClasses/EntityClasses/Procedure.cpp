#include "Procedure.h"

Procedure::Procedure(std::string *procedureName) : Entity(&Procedure::getEntityTypeStatic(), std::shared_ptr<std::string>(procedureName)) {
}

bool Procedure::operator==(const Procedure &procedure) const {
    return this->getEntityValue() == procedure.getEntityValue();
}

EntityType &Procedure::getEntityTypeStatic() {
    return Procedure::procedureType;
}

EntityType &Procedure::getEntityType() const {
    return Procedure::getEntityTypeStatic();
}

size_t std::hash<Procedure>::operator()(const Procedure &procedure) const {
    return std::hash<std::string *>()(procedure.getEntityValue());
}

EntityType Procedure::procedureType = EntityType();
