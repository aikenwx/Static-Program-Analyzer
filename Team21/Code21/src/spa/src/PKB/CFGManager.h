//
// Created by Aiken Wong on 7/3/23.
//

#ifndef SPA_CFGMANAGER_H
#define SPA_CFGMANAGER_H

#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "memory"
#include "sp/cfg/cfg.h"

class CFGManager {
   public:
    CFGManager() = default;
    void storeCFG(Procedure* procedure, std::shared_ptr<cfg::CFG> cfg);
};

#endif  // SPA_CFGMANAGER_H
