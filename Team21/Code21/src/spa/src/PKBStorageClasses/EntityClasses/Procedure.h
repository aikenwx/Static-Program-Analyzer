//
// Created by Aiken Wong on 2/2/23.
//

#ifndef SPA_PROCEDURE_H
#define SPA_PROCEDURE_H

#include <string>
#include "Entity.h"

class Procedure : public Entity {
private:
    std::string procedureName;

public:
    Procedure(std::string variableValue);

    std::string getEntityValue();

    bool equals(Entity* otherEntity);
};


#endif //SPA_PROCEDURE_H

