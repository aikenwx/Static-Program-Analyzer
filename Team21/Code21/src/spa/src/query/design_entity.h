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

DesignEntity getDesignEntity(std::string& deString);
std::string getString(DesignEntity designEntity);

}