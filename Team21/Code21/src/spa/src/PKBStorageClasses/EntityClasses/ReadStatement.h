#ifndef SPA_READSTATEMENT_H
#define SPA_READSTATEMENT_H

#include "Statement.h"
class ReadStatement : public Statement {
   private:
    static EntityType readStatementType;

   public:
    ReadStatement(int statementNumber);

    static EntityType& getEntityTypeStatic();

    EntityType& getEntityType() const override;
};

#endif
