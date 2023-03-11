#ifndef SPA_STATEMENT_H
#define SPA_STATEMENT_H

#include <memory>
#include <string>

#include "Entity.h"

class Statement : public Entity {
   private:
    static EntityType statementType;
    int statementNumber;

   public:
    Statement(int statementNumber);

    static EntityType& getEntityTypeStatic();
    static bool isStatement(Entity* entity);

    virtual ~Statement(){};

    int getStatementNumber() const;

    virtual EntityType& getEntityType() const override;
};

#endif
