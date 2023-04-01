#ifndef SPA_READSTATEMENT_H
#define SPA_READSTATEMENT_H

#include "Statement.h"
class ReadStatement : public Statement {
   private:
    static const StatementType readStatementType;

   public:
    explicit ReadStatement(int statementNumber);

    static auto getEntityTypeStatic() -> const EntityType &;

    [[nodiscard]] auto getEntityType() const -> const EntityType & override;
};

#endif
