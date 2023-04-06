#include "query/synonym.h"
#include <regex>
#include "query/query_exceptions.h"

namespace qps {

auto Synonym::isValidSynonym(const std::string &syn) -> bool {
  return std::regex_match(syn, std::regex("[a-zA-Z]([a-zA-Z]|\\d)*"));
}

auto Synonym::getSynonym() const -> const std::string & {
  return synonym;
}

Synonym::Synonym(const std::string &syn) : synonym{syn} {
  if (!Synonym::isValidSynonym(syn)) {
    throw QueryException(ErrorType::Syntactic, "Invalid synonym: " + syn);
  }
}
}  // namespace qps
