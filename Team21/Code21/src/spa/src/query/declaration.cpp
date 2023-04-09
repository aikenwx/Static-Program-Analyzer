#include "query/declaration.h"

#include <utility>

namespace qps {

Declaration::Declaration(DesignEntity design_entity_, Synonym synonym_)
    : design_entity{design_entity_}, synonym{std::move(std::move(synonym_))} {}

auto Declaration::getDesignEntity() const -> DesignEntity {
  return design_entity;
}

auto Declaration::getSynonym() const -> const Synonym & {
  return synonym;
}
}  // namespace qps
