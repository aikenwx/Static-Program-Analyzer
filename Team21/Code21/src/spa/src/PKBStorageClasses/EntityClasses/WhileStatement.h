#ifndef SPA_WHILESTATEMENT_H
#define SPA_WHILESTATEMENT_H


#include "Statement.h"


class WhileStatement : public Statement {
public:
    WhileStatement(int statementNumber);

    EntityType getEntityType() override;
};

#endif
