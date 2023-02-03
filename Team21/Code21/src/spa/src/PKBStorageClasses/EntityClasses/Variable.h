#ifndef SPA_VARIABLE_H
#define SPA_VARIABLE_H

#include <string>
#include "Entity.h"

class Variable : public Entity {
private:
    std::string variableName;

public:
    Variable(std::string variableValue);

    std::string getEntityValue();

    bool equals(Entity* otherEntity);
};


#endif //SPA_VARIABLE_H
