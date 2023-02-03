#include "WhileStatement.h"


WhileStatement::WhileStatement(int statementNumber)  {
    Statement::statementNumber = statementNumber;
}

bool WhileStatement::equals(Entity *otherEntity) {
    if (dynamic_cast<WhileStatement *>(otherEntity) != nullptr) {
        return dynamic_cast<WhileStatement *>(otherEntity)->statementNumber == this->statementNumber;
    }
    return false;
}





