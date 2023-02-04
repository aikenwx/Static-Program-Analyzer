#pragma once

#include <string>
#include <vector>

namespace qps {

class QueryParser {
	private:
		std::vector<std::string> tokens_;

	public:
		QueryParser(std::vector<std::string> tokens);

		bool dummyParse(int num);
};
}

