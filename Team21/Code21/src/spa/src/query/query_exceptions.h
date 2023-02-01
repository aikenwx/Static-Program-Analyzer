#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace qps {

class QueryException : public std::runtime_error {
public:
	QueryException(const std::string& msg = "") : std:runtime_error(msg) {}
};
}