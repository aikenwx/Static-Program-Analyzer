#include "query/design_entity.h"
#include "query/query_exceptions.h"

#include <stdexcept>
#include <map>

namespace qps {

std::map<DesignEntity, std::string> designEntityStringMap{ {
	{DesignEntity::STMT, "stmt"},
	{DesignEntity::READ, "read"},
	{DesignEntity::PRINT, "print"},
	{DesignEntity::CALL, "call"},
	{DesignEntity::WHILE, "while"},
	{DesignEntity::IF, "if"},
	{DesignEntity::ASSIGN, "assign"},
	{DesignEntity::VARIABLE, "variable"},
	{DesignEntity::CONSTANT, "constant"},
	{DesignEntity::PROCEDURE, "procedure"},
} };

std::map<std::string, DesignEntity> designEntityMap{ {
	{"stmt", DesignEntity::STMT},
	{"read", DesignEntity::READ},
	{"print", DesignEntity::PRINT},
	{"call", DesignEntity::CALL},
	{"while", DesignEntity::WHILE},
	{"if", DesignEntity::IF},
	{"assign", DesignEntity::ASSIGN},
	{"variable", DesignEntity::VARIABLE},
	{"constant", DesignEntity::CONSTANT},
	{"procedure", DesignEntity::PROCEDURE},
} };

DesignEntity getDesignEntityFromString(std::string deString) {
	try {
		return designEntityMap.at(deString);
	}
	catch (const std::out_of_range& err) {
		throw QueryException("Invalid Design Entity " + deString);
	}
}

std::string getStringFromDE(DesignEntity designEntity) {
	return designEntityStringMap.at(designEntity);
}

}
