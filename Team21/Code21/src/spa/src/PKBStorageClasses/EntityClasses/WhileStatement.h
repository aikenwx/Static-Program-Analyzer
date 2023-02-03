#ifndef SPA_WHILESTATEMENT_H
#define SPA_WHILESTATEMENT_H


#include "Statement.h"


class WhileStatement : public Statement {
public:
    bool equals(Entity *otherEntity) override;

    WhileStatement(int statementNumber);
};

#endif
