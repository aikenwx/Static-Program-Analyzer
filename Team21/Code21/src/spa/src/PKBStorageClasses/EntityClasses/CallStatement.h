#ifndef SPA_CALLSTATEMENT_H
#define SPA_CALLSTATEMENT_H

#include "Statement.h"
class CallStatement : public Statement {
   private:
    static EntityType const callStatementType;
    std::shared_ptr<std::string> procedureName;

   public:
    explicit CallStatement(int statementNumber);

    static auto getEntityTypeStatic() -> const EntityType &;

    [[nodiscard]] auto getEntityType() const -> const EntityType & override;

    auto getProcedureName() -> std::string*;

    void setProcedureName(std::shared_ptr<std::string> procedureNameString);
};

#endif
