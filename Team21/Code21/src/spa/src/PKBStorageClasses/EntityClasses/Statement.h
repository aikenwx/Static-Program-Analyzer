#ifndef SPA_STATEMENT_H
#define SPA_STATEMENT_H


#include "Entity.h"
#include <string>

class Statement : public Entity {

protected:
    int statementNumber;
public:
    virtual bool equals(Entity* otherEntity) override = 0;

    std::string getEntityValue() override;

};

#endif