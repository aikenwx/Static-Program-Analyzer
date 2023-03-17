#ifndef SPA_IFSTATEMENT_H
#define SPA_IFSTATEMENT_H

#include "Statement.h"

class IfStatement : public Statement {
   private:
    static EntityType ifStatementType;

   public:
    explicit IfStatement(int statementNumber);

    static auto getEntityTypeStatic() -> EntityType&;

    [[nodiscard]] auto getEntityType() const -> EntityType& override;
};

#endif
