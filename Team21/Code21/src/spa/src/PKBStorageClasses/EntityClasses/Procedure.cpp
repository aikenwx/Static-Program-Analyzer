#include "Procedure.h"

Procedure::Procedure(std::string procedureName) {
    this->procedureName = procedureName;
}

std::string Procedure::getEntityValue() {
    return this->procedureName;
}

bool Procedure::equals(Entity *otherEntity) {
    if (dynamic_cast<Procedure *>(otherEntity) != nullptr) {
        return dynamic_cast<Procedure *>(otherEntity)->procedureName == this->procedureName;
    }
    return false;
}
