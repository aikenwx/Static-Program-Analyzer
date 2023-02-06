#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace qps {

	enum class ErrorType { Semantic, Syntactic };

	class QueryException : public std::runtime_error {
	private:
		ErrorType type_;

	public:
		QueryException(ErrorType type_, const std::string& msg = "") : std::runtime_error(msg) {}

		ErrorType getType() {
			return type_;
		}
	};
}
