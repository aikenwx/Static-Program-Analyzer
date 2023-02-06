#include "parent_extractor.h"

#include "../rel/relationship.h"

namespace design_extractor {
ParentExtractor* ParentExtractor::instance_ = nullptr;

ParentExtractor* ParentExtractor::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new ParentExtractor();
  }
  return instance_;
}
}  // namespace design_extractor
