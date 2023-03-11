#include "query/attr_ref.h"

namespace qps {

	std::map<std::string, AttrName> attrNameMap = {
		{"procName", AttrName::ProcName},
		{"varName", AttrName::VarName},
		{"value", AttrName::Value},
		{"stmt#", AttrName::StmtNo},
	};

	std::map<AttrName, AttrType> attrTypeMap = {
		{AttrName::ProcName, AttrType::NAME},
		{AttrName::VarName, AttrType::NAME},
		{AttrName::Value, AttrType::INTEGER},
		{AttrName::StmtNo, AttrType::INTEGER},
	};

	std::map<DesignEntity, std::set<AttrName>> validAttrNameMap = {
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

	AttrType getAttrType(AttrName attrName) {
		return attrTypeMap.at(attrName);
	}

	AttrName getAttrNameFromString(std::string str) {
		try {
			return attrNameMap.at(str);
		}
		catch (const std::out_of_range& err) {
			throw QueryException(ErrorType::Syntactic, "Syntactic error. Invalid Attribute Name " + str);
		}
	}

	std::set<AttrName> getValidAttrNameSet(Declaration declaration) {
		DesignEntity de = declaration.getDesignEntity();
		return validAttrNameMap.at(de);
	}

	AttrRef::AttrRef(Synonym synonym_, AttrName attrName_)
		: synonym{ synonym_ }, attrName{ attrName_ } {}

}
