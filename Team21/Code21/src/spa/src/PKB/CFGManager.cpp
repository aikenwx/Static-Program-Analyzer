//
// Created by Aiken Wong on 7/3/23.
//

#include "CFGManager.h"

void CFGManager::storeCFG(Procedure* procedure, std::shared_ptr<cfg::CFG> cfg) {
    procedure->setCFG(cfg);
}
