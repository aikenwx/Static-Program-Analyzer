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
		explicit QueryException(ErrorType type_, const std::string& msg = "") : type_(type_), std::runtime_error(msg) {}

		auto getType() -> ErrorType {
			return type_;
		}
	};
}  // namespace qps
