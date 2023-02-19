#ifndef SPA_READSTATEMENT_H
#define SPA_READSTATEMENT_H

#include "Statement.h"

class ReadStatement : public Statement {
   public:
    ReadStatement(int statementNumber);

    EntityType getEntityType() override;
};

#endif
