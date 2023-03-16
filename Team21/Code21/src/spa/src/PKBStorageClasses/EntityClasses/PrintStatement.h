#ifndef SPA_PRINTSTATEMENT_H
#define SPA_PRINTSTATEMENT_H

#include "Statement.h"

class PrintStatement : public Statement {
   private:
    static EntityType printStatementType;

   public:
    explicit PrintStatement(int statementNumber);

    static auto getEntityTypeStatic() -> EntityType &;

    [[nodiscard]] auto getEntityType() const -> EntityType & override;
};

#endif
