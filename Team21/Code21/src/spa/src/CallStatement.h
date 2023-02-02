#ifndef SPA_CALLSTATEMENT_H
#define SPA_CALLSTATEMENT_H


#include "Statement.h"


class CallStatement : public Statement {
private:
    int statementNumber;
public:
    bool equals(Entity *otherEntity) override;

    CallStatement(int statementNumber);
};

#endif
