//
// Created by Aiken Wong on 2/2/23.
//
#include <string>

#include "Entity.h"

#ifndef SPA_CONSTANT_H
#define SPA_CONSTANT_H

class Constant : public Entity {
   private:
    int constantValue;
    std::shared_ptr<std::string> constantValueString;

   public:
    Constant(int constantValue);

    ~Constant() = default;

    std::string * getEntityValue() override;

    EntityType getEntityType() override;

    int getConstantNumber();
};

#endif  // SPA_CONSTANT_H
