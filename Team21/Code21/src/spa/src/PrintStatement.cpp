#include "PrintStatement.h"

PrintStatement::PrintStatement(int statementNumber) {
    this->statementNumber = statementNumber;

}

bool PrintStatement::equals(Entity *otherEntity) {
    if (dynamic_cast<PrintStatement *>(otherEntity) != nullptr) {
        return dynamic_cast<PrintStatement *>(otherEntity)->statementNumber == this->statementNumber;
    }
    return false;
}





