#include "query/quoted_identifier.h"
#include "query/query_exceptions.h"

#include <regex>

namespace qps {

    bool QuotedIdentifier::isValidId(std::string id) {
        return std::regex_match(id, std::regex("[a-zA-Z]([a-zA-Z]|\\d)*"));
    }

    QuotedIdentifier::QuotedIdentifier(std::string id_) : id{ id_ } {
        if (!QuotedIdentifier::isValidId(id_)) {
            throw QueryException("Invalid quoted identifier: " + id_);
        }
    }

    std::string QuotedIdentifier::getQuotedId() {
        return id;
    }

}
