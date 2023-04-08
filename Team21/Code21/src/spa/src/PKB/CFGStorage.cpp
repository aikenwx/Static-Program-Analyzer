//
// Created by Aiken Wong on 7/3/23.
//

#include "CFGStorage.h"

#include <utility>

void CFGStorage::storeCFG(std::shared_ptr<cfg::CFG> cfg) {
  this->cfg = std::move(cfg);
}

auto CFGStorage::getCFG() -> cfg::CFG* { return this->cfg.get(); }
