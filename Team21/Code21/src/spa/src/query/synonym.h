#pragma once

#include <iostream>
#include <string>

namespace qps {

// Synonym class represents a PQL synonym.
class Synonym {
	private:
		std::string synonym;

	public:
		Synonym(std::string syn);
		std::string getSynonym();
		static bool isValidSynonym(std::string syn);

		bool operator==(const Synonym& anotherSyn) const {
			return synonym == anotherSyn.getSynonym();
		}

		friend std::ostream& operator<<(std::ostream& os, Synonym const& syn) {
			os << syn.synonym;
			return os;
		}
};
}
