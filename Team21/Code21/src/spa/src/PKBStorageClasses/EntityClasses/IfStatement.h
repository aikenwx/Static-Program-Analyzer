#ifndef SPA_IFSTATEMENT_H
#define SPA_IFSTATEMENT_H

#include "Statement.h"

class IfStatement : public Statement {
   public:
    IfStatement(int statementNumber);

    EntityType getEntityType() override;
};

#endif
