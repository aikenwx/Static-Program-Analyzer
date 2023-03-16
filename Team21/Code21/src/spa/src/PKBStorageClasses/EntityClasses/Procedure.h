//
// Created by Aiken Wong on 2/2/23.
//

#ifndef SPA_PROCEDURE_H
#define SPA_PROCEDURE_H

#include <memory>
#include <string>

#include "Entity.h"
#include "sp/cfg/cfg.h"

class Procedure : public Entity {
   private:
    static EntityType procedureType;
    size_t hash{};

    std::shared_ptr<std::string> procedureName;

   public:
    static auto getEntityTypeStatic() -> EntityType&;

    [[nodiscard]] auto getEntityType() const -> EntityType& override;

    explicit Procedure(std::string* variableValue);

    ~Procedure() override = default;

    auto operator==(const Procedure& procedure) const -> bool;
};

template <>
struct std::hash<Procedure> {
  auto operator()(const Procedure& procedure) const -> size_t;
};

#endif  // SPA_PROCEDURE_H
