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
    std::shared_ptr<std::string> procedureName;

    std::shared_ptr<cfg::CFG> cfg;
    
   public:
    Procedure(std::string* variableValue);

    ~Procedure() = default;

    void setCFG(std::shared_ptr<cfg::CFG> cfg);

    cfg::CFG * getCFG();

    std::string * getEntityValue() override;

    EntityType getEntityType() override;
};

#endif  // SPA_PROCEDURE_H
