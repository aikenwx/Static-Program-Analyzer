#ifndef SPA_READSTATEMENT_H
#define SPA_READSTATEMENT_H

#include "Statement.h"
class ReadStatement : public Statement {
   private:
    static EntityType readStatementType;

   public:
    explicit ReadStatement(int statementNumber);

    static auto getEntityTypeStatic() -> EntityType&;

    [[nodiscard]] auto getEntityType() const -> EntityType& override;
};

#endif
