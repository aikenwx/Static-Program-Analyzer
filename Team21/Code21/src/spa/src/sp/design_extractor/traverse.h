#include "../rel/relationship.h"
#include "extractor.h"

namespace design_extractor {
std::vector<rel::Relationship*> Traverse(ast::INode* node, Extractor* extractor);
}  // namespace design_extractor
