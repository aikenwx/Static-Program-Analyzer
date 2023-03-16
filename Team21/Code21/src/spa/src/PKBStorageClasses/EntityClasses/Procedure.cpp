#include "Procedure.h"

Procedure::Procedure(std::string *procedureName) : Entity(&Procedure::getEntityTypeStatic(), std::shared_ptr<std::string>(procedureName)) {
}

auto Procedure::operator==(const Procedure &procedure) const -> bool {
  return this->getEntityValue() == procedure.getEntityValue();
}

auto Procedure::getEntityTypeStatic() -> EntityType & {
  return Procedure::procedureType;
}

auto Procedure::getEntityType() const -> EntityType & {
  return Procedure::getEntityTypeStatic();
}

auto std::hash<Procedure>::operator()(const Procedure &procedure) const
    -> size_t {
  return std::hash<std::string *>()(procedure.getEntityValue());
}

EntityType Procedure::procedureType = EntityType();
