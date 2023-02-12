//
// Created by Aiken Wong on 2/2/23.
//

#ifndef SPA_PROCEDURE_H
#define SPA_PROCEDURE_H

#include <memory>
#include <string>

#include "Entity.h"

class Procedure : public Entity {
   private:
    std::shared_ptr<std::string> procedureName;

   public:
    Procedure(std::string* variableValue);

    ~Procedure() = default;

    std::string * getEntityValue() override;

    EntityType getEntityType() override;
};

#endif  // SPA_PROCEDURE_H
