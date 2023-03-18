#ifndef SPA_VARIABLE_H
#define SPA_VARIABLE_H

#include <memory>
#include <string>

#include "Entity.h"

class Variable : public Entity {
   private:
    static const EntityType variableType;
    size_t hash{};

   public:
    static auto getEntityTypeStatic() -> const EntityType &;

    explicit Variable(const std::shared_ptr<std::string>& variableValue);

    ~Variable() override = default;

    [[nodiscard]] auto getEntityType() const -> const EntityType & override;
};

#endif  // SPA_VARIABLE_H
