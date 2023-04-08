//
// Created by Aiken Wong on 7/3/23.
//

#ifndef SPA_CFGSTORAGE_H
#define SPA_CFGSTORAGE_H

#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "memory"
#include "sp/cfg/cfg.h"

class CFGStorage {
private:
    std::shared_ptr<cfg::CFG> cfg;

   public:
    CFGStorage() = default;
    void storeCFG(std::shared_ptr<cfg::CFG> cfg);

    auto getCFG() -> cfg::CFG*;
};



#endif  // SPA_CFGSTORAGE_H
