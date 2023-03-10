#ifndef SPA_CALLSTATEMENT_H
#define SPA_CALLSTATEMENT_H

#include "Statement.h"
class CallStatement : public Statement {
   private:
    static EntityType callStatementType;

   public:
    CallStatement(int statementNumber);

    static EntityType& getEntityTypeStatic();

    EntityType& getEntityType() const override;
};

#endif
