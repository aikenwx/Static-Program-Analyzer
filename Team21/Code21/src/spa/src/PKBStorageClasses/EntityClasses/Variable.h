#ifndef SPA_VARIABLE_H
#define SPA_VARIABLE_H

#include <memory>
#include <string>

#include "Entity.h"

class Variable : public Entity {
   private:
    std::shared_ptr<std::string> variableName;

   public:
    Variable(std::string* variableValue);

    std::string * getEntityValue() override;

    EntityType getEntityType() override;
    ;
};

#endif  // SPA_VARIABLE_H
