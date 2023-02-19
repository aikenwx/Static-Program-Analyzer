#ifndef SPA_ASSIGNSTATEMENT_H
#define SPA_ASSIGNSTATEMENT_H

#include <memory>

#include "Statement.h"

class AssignStatement : public Statement {
   private:
    std::shared_ptr<std::string> postFixExpression;

   public:
    AssignStatement(int statementNumber);

    void setPostfixExpression(std::string* postfixExpression);

    EntityType getEntityType() override;

    std::string* getPostFixExpression();
};

#endif
