#include "IfStatement.h"


IfStatement::IfStatement(int statementNumber) {
    this->statementNumber = statementNumber;
}

bool IfStatement::equals(Entity *otherEntity) {
    if (dynamic_cast<IfStatement *>(otherEntity) != nullptr) {
        return dynamic_cast<IfStatement *>(otherEntity)->statementNumber == this->statementNumber;
    }
    return false;
}





