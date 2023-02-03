#include "AssignStatement.h"


AssignStatement::AssignStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
}

bool AssignStatement::equals(Entity *otherEntity) {
    if (dynamic_cast<AssignStatement *>(otherEntity) != nullptr) {
        return dynamic_cast<AssignStatement *>(otherEntity)->statementNumber == this->statementNumber;
    }
    return false;
}





