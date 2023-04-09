#include "query/quoted_identifier.h"
#include "query/query_exceptions.h"

#include <regex>

namespace qps {

// Check if id for QuotedIdentifier is syntactically correct or not
auto QuotedIdentifier::isValidId(const std::string& iden) -> bool {
  return std::regex_match(iden, std::regex("[a-zA-Z]([a-zA-Z]|\\d)*"));
}

QuotedIdentifier::QuotedIdentifier(const std::string& id_) : id{ id_ } {
  if (!QuotedIdentifier::isValidId(id_)) {
    throw QueryException(ErrorType::Syntactic, "Invalid quoted identifier: " + id_);
  }
}

auto QuotedIdentifier::getQuotedId() const -> const std::string & {
  return id;
}

}  // namespace qps
