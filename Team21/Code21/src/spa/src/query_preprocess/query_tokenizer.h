#pragma once

#include <string>

namespace qps {

class QueryTokenizer {
	private:
		std::string currentStr; //Current String for QueryLexer

	public:
		QueryTokenizer(std::string source);

		bool dummyTokenize(int num);

};
}
