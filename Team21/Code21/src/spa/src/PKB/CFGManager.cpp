//
// Created by Aiken Wong on 7/3/23.
//

#include "CFGManager.h"

#include <utility>

void CFGManager::storeCFG(std::shared_ptr<cfg::CFG> cfg) {
  this->cfg = std::move(cfg);
}

auto CFGManager::getCFG() -> cfg::CFG* { return this->cfg.get(); }
