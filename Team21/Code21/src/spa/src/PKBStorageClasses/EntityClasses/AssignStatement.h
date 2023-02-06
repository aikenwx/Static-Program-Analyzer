#ifndef SPA_ASSIGNSTATEMENT_H
#define SPA_ASSIGNSTATEMENT_H


#include "Statement.h"


class AssignStatement : public Statement {
public:
    AssignStatement(int statementNumber);

    EntityType getEntityType() override;
};

#endif
