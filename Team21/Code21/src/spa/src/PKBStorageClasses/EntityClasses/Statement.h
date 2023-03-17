#ifndef SPA_STATEMENT_H
#define SPA_STATEMENT_H

#include <memory>
#include <string>

#include "Entity.h"

class Statement : public Entity {
   private:
    static const EntityType statementType;
    int statementNumber;

   public:
    Statement(const EntityType *entityType, int statementNumber);

    static auto getEntityTypeStatic() -> const EntityType&;
    static auto isStatement(Entity* entity) -> bool;

    ~Statement() override = default;
    ;

    [[nodiscard]] auto getStatementNumber() const -> int;

    [[nodiscard]] auto getEntityType() const -> const EntityType& override;
};

#endif
