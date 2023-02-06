#include "proc_relationship.h"

namespace rel {
std::string ProcRelationship::procedureName() { return procedureName_; }

ProcRelationship ProcRelationship::CreateRelationship(std::string procedureName) {
  return ProcRelationship(procedureName);
};

ProcRelationship::ProcRelationship(std::string procedureName) {
  procedureName_ = procedureName;
};
}
