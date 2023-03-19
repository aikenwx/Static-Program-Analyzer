#pragma once

#include <string>

namespace qps {

// Enum class DesignEntity represents all possible design entity in PQL
enum class DesignEntity {
	STMT,
	READ,
	PRINT,
	CALL,
	WHILE,
	IF,
	ASSIGN,
	VARIABLE,
	CONSTANT,
	PROCEDURE,

};

auto getDesignEntityFromString(const std::string& deString) -> DesignEntity;
auto getStringFromDE(DesignEntity designEntity) -> std::string;

}  // namespace qps
