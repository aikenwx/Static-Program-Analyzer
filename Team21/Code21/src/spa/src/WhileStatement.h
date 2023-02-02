#ifndef SPA_WHILESTATEMENT_H
#define SPA_WHILESTATEMENT_H


#include "Statement.h"


class WhileStatement : public Statement {
private:
    int statementNumber;
public:
    bool equals(Entity *otherEntity) override;

    WhileStatement(int statementNumber);
};

#endif
