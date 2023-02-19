#include "proc_relationship.h"

namespace rel {
std::string ProcRelationship::procedureName() { return procedureName_; }

std::unique_ptr<ProcRelationship> ProcRelationship::CreateRelationship(std::string procedureName) {
  return std::unique_ptr<ProcRelationship>(new ProcRelationship(procedureName));
};

ProcRelationship::ProcRelationship(std::string procedureName) {
  procedureName_ = procedureName;
};
}
