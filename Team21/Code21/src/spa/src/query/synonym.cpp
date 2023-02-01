#include "query/synonym.h"
#include <regex>
#include "query/query_exceptions.h"

namespace qps {

bool Synonym::isValidSynonym(std::string syn) {
	return std::regex_match(syn, std::regex("[a-zA-Z]([a-zA-Z]|\\d)*"));
}

Synonym::Synonym(std::string syn) : synonym{ syn } {
	if (!Synonym::isValidSynonym(syn)) {
		throw QueryException("Invalid synonym: " + syn);
	}
}
}