#include "Procedure.h"

Procedure::Procedure(std::string* procedureName) {
    this->procedureName = std::shared_ptr<std::string>(procedureName);
}

std::string * Procedure::getEntityValue() {
    return this->procedureName.get();
}

EntityType Procedure::getEntityType() {
    return EntityType::PROCEDURE;
}

void Procedure::setCFG(std::shared_ptr<cfg::CFG> cfg) {
    this->cfg = cfg;
}

cfg::CFG * Procedure::getCFG() {
    return this->cfg.get();
}
