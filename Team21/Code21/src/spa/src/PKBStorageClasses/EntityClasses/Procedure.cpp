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
