#ifndef SPA_CALLSTATEMENT_H
#define SPA_CALLSTATEMENT_H

#include "Statement.h"

class CallStatement : public Statement {
   public:
    CallStatement(int statementNumber);

    EntityType getEntityType() override;
};

#endif
