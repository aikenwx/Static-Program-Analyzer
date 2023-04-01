#ifndef SPA_WHILESTATEMENT_H
#define SPA_WHILESTATEMENT_H

#include "Statement.h"

class WhileStatement : public Statement {
   private:
    static const StatementType whileStatementType;

   public:
    explicit WhileStatement(int statementNumber);

    static auto getEntityTypeStatic() -> const EntityType &;

    [[nodiscard]] auto getEntityType() const -> const EntityType & override;
};

#endif
