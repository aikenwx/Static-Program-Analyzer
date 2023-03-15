#ifndef SPA_VARIABLE_H
#define SPA_VARIABLE_H

#include <memory>
#include <string>

#include "Entity.h"

class Variable : public Entity {
   private:
    static EntityType variableType;
    size_t hash;

   public:
    static EntityType& getEntityTypeStatic();

    Variable(std::string* variableValue);

    ~Variable() = default;

    EntityType& getEntityType() const override;
};

#endif  // SPA_VARIABLE_H
