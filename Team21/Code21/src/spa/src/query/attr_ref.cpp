#include <utility>

#include "query/attr_ref.h"

namespace qps {

// Look up table from query substring to Attribute Name
const std::map<std::string, AttrName> attrNameMap = {
	{"procName", AttrName::ProcName},
	{"varName", AttrName::VarName},
	{"value", AttrName::Value},
	{"stmt#", AttrName::StmtNo},
};

// Look up table from Attribute Name to Attribute Type
const std::map<AttrName, AttrType> attrTypeMap = {
	{AttrName::ProcName, AttrType::NAME},
	{AttrName::VarName, AttrType::NAME},
	{AttrName::Value, AttrType::INTEGER},
	{AttrName::StmtNo, AttrType::INTEGER},
};

// Look up table from Design Entity to set of valid Attribute Name for that design entity
const std::map<DesignEntity, std::set<AttrName>> validAttrNameMap = {
	{DesignEntity::STMT, {AttrName::StmtNo}},
	{DesignEntity::READ, {AttrName::StmtNo, AttrName::VarName}},
	{DesignEntity::PRINT, {AttrName::StmtNo, AttrName::VarName}},
	{DesignEntity::CALL, {AttrName::StmtNo, AttrName::ProcName}},
	{DesignEntity::WHILE, {AttrName::StmtNo}},
	{DesignEntity::IF, {AttrName::StmtNo}},
	{DesignEntity::ASSIGN, {AttrName::StmtNo}},
	{DesignEntity::VARIABLE, {AttrName::VarName}},
	{DesignEntity::CONSTANT, {AttrName::Value}},
	{DesignEntity::PROCEDURE, {AttrName::ProcName}},
};

// Get Attribute Type with given Attribute Name
auto getAttrType(AttrName attrName) -> AttrType {
	return attrTypeMap.at(attrName);
}

// Get Attribute Name with given string when parsing
auto getAttrNameFromString(const std::string& str) -> AttrName {
	try {
		return attrNameMap.at(str);
	}
	catch (const std::out_of_range& err) {
		throw QueryException(ErrorType::Syntactic, "Syntactic error. Invalid Attribute Name " + str);
	}
}

// Get list of valid Attribute Name with given synonym (declaration)
auto getValidAttrNameSet(const Declaration &declaration) -> const std::set<AttrName> & {
  DesignEntity entity = declaration.getDesignEntity();
  return validAttrNameMap.at(entity);
}

AttrRef::AttrRef(Synonym synonym_, AttrName attrName_)
    : synonym{std::move(synonym_)}, attrName{attrName_} {}

}  // namespace qps
