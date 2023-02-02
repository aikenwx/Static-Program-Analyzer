#ifndef SPA_PRINTSTATEMENT_H
#define SPA_PRINTSTATEMENT_H


#include "Statement.h"

class PrintStatement : public Statement {
private:
    int statementNumber;
public:
    bool equals(Entity *otherEntity) override;

    PrintStatement(int statementNumber);
};

#endif
