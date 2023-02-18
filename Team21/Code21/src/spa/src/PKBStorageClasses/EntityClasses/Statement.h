#ifndef SPA_STATEMENT_H
#define SPA_STATEMENT_H

#include <memory>
#include <string>

#include "Entity.h"

class Statement : public Entity {
   protected:
    int statementNumber;
    std::shared_ptr<std::string> statementNumberString;

   public:

    virtual ~Statement() {};
    std::string * getEntityValue() override;

    const int getStatementNumber();

    virtual EntityType getEntityType() override = 0;
};

#endif
