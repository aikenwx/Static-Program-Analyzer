#ifndef SPA_ASSIGNSTATEMENT_H
#define SPA_ASSIGNSTATEMENT_H


#include "Statement.h"


class AssignStatement : public Statement {
public:
    bool equals(Entity *otherEntity) override;

    AssignStatement(int statementNumber);
};

#endif
