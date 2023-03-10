#ifndef SPA_IFSTATEMENT_H
#define SPA_IFSTATEMENT_H

#include "Statement.h"

class IfStatement : public Statement {
   private:
    static EntityType ifStatementType;

   public:
    IfStatement(int statementNumber);

    static EntityType& getEntityTypeStatic();

    EntityType& getEntityType() const override;
};

#endif
