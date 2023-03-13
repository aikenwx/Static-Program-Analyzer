#ifndef SPA_ASSIGNSTATEMENT_H
#define SPA_ASSIGNSTATEMENT_H

#include <memory>

#include "Statement.h"

class AssignStatement : public Statement {
   private:
    static EntityType assignStatementType;

    std::shared_ptr<std::string> postFixExpression;

   public:
    AssignStatement(int statementNumber);

    static EntityType &getEntityTypeStatic();

    EntityType &getEntityType() const override;

    void setPostfixExpression(std::string *postfixExpression);

    std::string *getPostFixExpression();
};

#endif
