#include "query/synonym.h"
#include <regex>
#include "query/query_exceptions.h"

namespace qps {

bool Synonym::isValidSynonym(std::string syn) {
	return std::regex_match(syn, std::regex("[a-zA-Z]([a-zA-Z]|\\d)*"));
}

std::string Synonym::getSynonym() {
	return synonym;
}

Synonym::Synonym(std::string syn) : synonym{ syn } {
	if (!Synonym::isValidSynonym(syn)) {
		throw QueryException(ErrorType::Syntactic, "Invalid synonym: " + syn);
	}
}
}
