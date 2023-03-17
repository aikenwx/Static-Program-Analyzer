#ifndef SPA_WHILESTATEMENT_H
#define SPA_WHILESTATEMENT_H

#include "Statement.h"

class WhileStatement : public Statement {
   private:
    static EntityType whileStatementType;

   public:
    explicit WhileStatement(int statementNumber);

    static auto getEntityTypeStatic() -> EntityType&;

    [[nodiscard]] auto getEntityType() const -> EntityType& override;
};

#endif
