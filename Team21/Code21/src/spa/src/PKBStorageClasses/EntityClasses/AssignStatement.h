#ifndef SPA_ASSIGNSTATEMENT_H
#define SPA_ASSIGNSTATEMENT_H

#include <memory>

#include "Statement.h"

class AssignStatement : public Statement {
   private:
    static const StatementType assignStatementType;

    std::shared_ptr<std::string> postFixExpression;

   public:
    explicit AssignStatement(int statementNumber);

    static auto getEntityTypeStatic() -> const EntityType &;

    [[nodiscard]] auto getEntityType() const -> const EntityType & override;

    void setPostfixExpression(std::shared_ptr<std::string> postfixExpression);

    auto getPostFixExpression() -> std::string *;
};

#endif
