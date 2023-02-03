#include "ReadStatement.h"

ReadStatement::ReadStatement(int statementNumber)  {
    Statement::statementNumber = statementNumber;
}

bool ReadStatement::equals(Entity *otherEntity) {
    if (dynamic_cast<ReadStatement *>(otherEntity) != nullptr) {
        return dynamic_cast<ReadStatement *>(otherEntity)->statementNumber == this->statementNumber;
    }
    return false;
}
