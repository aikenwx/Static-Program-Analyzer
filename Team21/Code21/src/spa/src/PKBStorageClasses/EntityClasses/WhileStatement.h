#ifndef SPA_WHILESTATEMENT_H
#define SPA_WHILESTATEMENT_H

#include "Statement.h"

class WhileStatement : public Statement {
   private:
    static EntityType whileStatementType;

   public:
    WhileStatement(int statementNumber);

    static EntityType& getEntityTypeStatic();

    EntityType& getEntityType() const override;
};

#endif
