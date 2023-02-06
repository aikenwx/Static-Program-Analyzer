#ifndef SPA_STATEMENT_H
#define SPA_STATEMENT_H


#include "Entity.h"
#include <string>

class Statement : public Entity {

protected:
    int statementNumber;
public:
    std::string getEntityValue() override;

    virtual EntityType getEntityType() override = 0;
};

#endif
