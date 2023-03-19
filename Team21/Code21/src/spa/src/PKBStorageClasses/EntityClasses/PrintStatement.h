#ifndef SPA_PRINTSTATEMENT_H
#define SPA_PRINTSTATEMENT_H

#include "Statement.h"

class PrintStatement : public Statement {
   private:
    static const EntityType printStatementType;

   public:
    explicit PrintStatement(int statementNumber);

    static auto getEntityTypeStatic() -> const EntityType &;

    [[nodiscard]] auto getEntityType() const -> const EntityType & override;
};

#endif
