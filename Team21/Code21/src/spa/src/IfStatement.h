#ifndef SPA_IFSTATEMENT_H
#define SPA_IFSTATEMENT_H


#include "Statement.h"


class IfStatement : public Statement {
private:
    int statementNumber;
public:
    bool equals(Entity *otherEntity) override;

    IfStatement(int statementNumber);
};

#endif
