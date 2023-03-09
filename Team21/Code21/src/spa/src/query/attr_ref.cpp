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
			throw QueryException(ErrorType::Syntactic, "Invalid Attribute Name " + str);
		}
	}

	std::set<AttrName> getValidAttrNameSet(Declaration declaration) {
		DesignEntity de = declaration.getDesignEntity();
		return validAttrNameMap.at(de);
	}

	AttrRef::AttrRef(Declaration synonym_decl_, AttrName attrName_)
		: synonym_decl{ synonym_decl_ }, attrName{ attrName_ } {
		std::set<AttrName> validAttrNameSet = getValidAttrNameSet(synonym_decl_);
		if (validAttrNameSet.find(attrName_) == validAttrNameSet.end()) {
			throw QueryException(ErrorType::Syntactic, "Attribute name is invalid with synonym " + synonym_decl.getSynonym().getSynonym());
		}
	}

}