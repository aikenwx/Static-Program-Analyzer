#ifndef SPA_CALLSTATEMENT_H
#define SPA_CALLSTATEMENT_H

#include "Statement.h"
class CallStatement : public Statement {
   private:
    static EntityType callStatementType;
    std::shared_ptr<std::string> procedureName;

   public:
    CallStatement(int statementNumber);

    static EntityType& getEntityTypeStatic();

    EntityType& getEntityType() const override;

    std::string* getProcedureName();

    void setProcedureName(std::string* procedureName);
};

#endif
