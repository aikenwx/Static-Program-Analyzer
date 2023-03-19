#include <utility>


#include "query/attr_ref.h"

namespace qps {

	const std::map<std::string, AttrName> attrNameMap = {
		{"procName", AttrName::ProcName},
		{"varName", AttrName::VarName},
		{"value", AttrName::Value},
		{"stmt#", AttrName::StmtNo},
	};

	const std::map<AttrName, AttrType> attrTypeMap = {
		{AttrName::ProcName, AttrType::NAME},
		{AttrName::VarName, AttrType::NAME},
		{AttrName::Value, AttrType::INTEGER},
		{AttrName::StmtNo, AttrType::INTEGER},
	};

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

	auto getAttrType(AttrName attrName) -> AttrType {
		return attrTypeMap.at(attrName);
	}

	auto getAttrNameFromString(const std::string& str) -> AttrName {
		try {
			return attrNameMap.at(str);
		}
		catch (const std::out_of_range& err) {
			throw QueryException(ErrorType::Syntactic, "Syntactic error. Invalid Attribute Name " + str);
		}
	}

	auto getValidAttrNameSet(Declaration declaration) -> std::set<AttrName> {
		DesignEntity entity = declaration.getDesignEntity();
		return validAttrNameMap.at(entity);
	}

	AttrRef::AttrRef(Synonym synonym_, AttrName attrName_)
		: synonym{std::move(std::move( synonym_ ))}, attrName{ attrName_ } {}

}  // namespace qps
