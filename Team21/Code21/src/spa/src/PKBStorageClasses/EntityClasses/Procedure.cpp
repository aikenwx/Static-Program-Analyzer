#include "Procedure.h"

Procedure::Procedure(std::string *procedureName) : Entity(&Procedure::getEntityTypeStatic(), std::shared_ptr<std::string>(procedureName)) {
}

void Procedure::setCFG(std::shared_ptr<cfg::CFG> cfg) {
    this->cfg = cfg;
}

cfg::CFG *Procedure::getCFG() {
    return this->cfg.get();
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
