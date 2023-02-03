#include "CallStatement.h"


CallStatement::CallStatement(int statementNumber) {
    Statement::statementNumber = statementNumber;
}


bool CallStatement::equals(Entity *otherEntity) {
    if (dynamic_cast<CallStatement *>(otherEntity) != nullptr) {
        return dynamic_cast<CallStatement *>(otherEntity)->statementNumber == this->statementNumber;
    }
    return false;
}





