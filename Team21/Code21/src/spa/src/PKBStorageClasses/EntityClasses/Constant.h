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
    static const EntityType entityType;

   public:
    static auto getEntityTypeStatic() -> const EntityType &;

    explicit Constant(int constantValue);

    ~Constant() override = default;

    [[nodiscard]] auto getEntityType() const -> const EntityType & override;
};

#endif  // SPA_CONSTANT_H
