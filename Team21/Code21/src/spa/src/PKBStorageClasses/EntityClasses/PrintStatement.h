#ifndef SPA_PRINTSTATEMENT_H
#define SPA_PRINTSTATEMENT_H


#include "Statement.h"

class PrintStatement : public Statement {
public:
    PrintStatement(int statementNumber);

    EntityType getEntityType() override;
};

#endif
