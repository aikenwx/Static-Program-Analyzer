//
// Created by Aiken Wong on 2/2/23.
//
#include <memory>
#include <string>

#include "Entity.h"

#ifndef SPA_CONSTANT_H
#define SPA_CONSTANT_H

class Constant : public Entity {
   private:
    static EntityType entityType;
    int constantValue;
    std::shared_ptr<std::string> constantValueString;

   public:
    static EntityType &getEntityTypeStatic();

    Constant(int constantValue);

    ~Constant() = default;

    EntityType &getEntityType() const override;

    int getConstantNumber() const;
};

#endif  // SPA_CONSTANT_H
