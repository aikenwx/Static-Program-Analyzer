#ifndef SPA_PRINTSTATEMENT_H
#define SPA_PRINTSTATEMENT_H

#include "Statement.h"

class PrintStatement : public Statement {
   private:
    static EntityType printStatementType;

   public:
    PrintStatement(int statementNumber);

    static EntityType &getEntityTypeStatic();

    EntityType &getEntityType() const override;
};

#endif
