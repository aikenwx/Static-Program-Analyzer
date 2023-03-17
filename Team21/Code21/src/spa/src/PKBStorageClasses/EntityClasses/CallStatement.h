#ifndef SPA_CALLSTATEMENT_H
#define SPA_CALLSTATEMENT_H

#include "Statement.h"
class CallStatement : public Statement {
   private:
    static EntityType callStatementType;
    std::shared_ptr<std::string> procedureName;

   public:
    explicit CallStatement(int statementNumber);

    static auto getEntityTypeStatic() -> EntityType&;

    [[nodiscard]] auto getEntityType() const -> EntityType& override;

    auto getProcedureName() -> std::string*;

    void setProcedureName(std::shared_ptr<std::string> procedureNameString);
};

#endif
