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
    size_t hash;

    std::shared_ptr<std::string> procedureName;

   public:
    static EntityType& getEntityTypeStatic();

    EntityType& getEntityType() const override;

    Procedure(std::string* variableValue);

    ~Procedure() = default;

    bool operator==(const Procedure& procedure) const;
};

template <>
struct std::hash<Procedure> {
    size_t operator()(const Procedure& procedure) const;
};

#endif  // SPA_PROCEDURE_H
