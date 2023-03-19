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
    static const EntityType procedureType;
    size_t hash{};

   public:
    static auto getEntityTypeStatic() -> const EntityType &;

    [[nodiscard]] auto getEntityType() const -> const EntityType & override;

    explicit Procedure(const std::shared_ptr<std::string>& procedureName);

    ~Procedure() override = default;

    auto operator==(const Procedure& procedure) const -> bool;
};

template <>
struct std::hash<Procedure> {
  auto operator()(const Procedure& procedure) const -> size_t;
};

#endif  // SPA_PROCEDURE_H
