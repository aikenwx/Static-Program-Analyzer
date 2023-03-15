//
// Created by Aiken Wong on 7/3/23.
//

#ifndef SPA_CFGMANAGER_H
#define SPA_CFGMANAGER_H

#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "memory"
#include "sp/cfg/cfg.h"

class CFGManager {
private:
    std::shared_ptr<cfg::CFG> cfg;

   public:
    CFGManager() = default;
    void storeCFG(std::shared_ptr<cfg::CFG> cfg);

    cfg::CFG* getCFG();
};



#endif  // SPA_CFGMANAGER_H
