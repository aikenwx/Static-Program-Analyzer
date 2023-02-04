#include "Procedure.h"

Procedure::Procedure(std::string procedureName) {
    this->procedureName = procedureName;
}

std::string Procedure::getEntityValue() {
    return this->procedureName;
}

EntityType Procedure::getEntityType() {
    return EntityType::PROCEDURE;
}
