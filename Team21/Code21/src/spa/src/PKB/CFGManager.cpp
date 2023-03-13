//
// Created by Aiken Wong on 7/3/23.
//

#include "CFGManager.h"

void CFGManager::storeCFG(std::shared_ptr<cfg::CFG> cfg) {
    this->cfg = cfg;
}

cfg::CFG* CFGManager::getCFG() {
    return this->cfg.get();
}
