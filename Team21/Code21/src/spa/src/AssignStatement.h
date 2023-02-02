#ifndef SPA_ASSIGNSTATEMENT_H
#define SPA_ASSIGNSTATEMENT_H


#include "Statement.h"


class AssignStatement : public Statement {
private:
    int statementNumber;
public:
    bool equals(Entity *otherEntity) override;

    AssignStatement(int statementNumber);
};

#endif
