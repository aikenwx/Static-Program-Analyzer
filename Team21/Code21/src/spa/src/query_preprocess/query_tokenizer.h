#pragma once

#include <string>
#include <vector>

namespace qps {

class QueryTokenizer {
	private:
		std::string queryString;
		int currentIndex;
		std::string currentString; //Current String for QueryTokenizer
		std::vector<std::string> tokens;

		char peek();
		char next();
		bool isEnd();
		void readPhrase();
		void readNumber();

	public:
		QueryTokenizer(std::string source);
		std::vector<std::string> tokenize();
};
}
