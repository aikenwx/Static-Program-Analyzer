#include "../rel/relationship.h"
#include "extractor.h"

namespace design_extractor {
std::vector<std::unique_ptr<rel::Relationship>> Traverse(std::shared_ptr<ast::INode> node, std::shared_ptr<Extractor> extractor);
}  // namespace design_extractor
