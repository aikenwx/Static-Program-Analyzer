#include "select_evaluator.h"

//TODO(DHRUV): Select Evaluation

namespace qps {
ClauseResult SelectEvaluator::ConstructResult(const std::vector<Entity *> &entities) {
  SynonymTable clause_result({synonym_});
  for (auto entity : entities) {
    clause_result.AddRow({entity});
  }
  return clause_result;
}

ClauseResult SelectEvaluator::Evaluate(QueryFacade &pkb) {
  std::vector<Entity *> entities;

  auto add_entity = [&](auto vec) {
    entities.insert(entities.end(), vec.begin(), vec.end());
  };

  switch (Declaration::findDeclarationWithSynonym(declarations_, synonym_).value().getDesignEntity()) {
    case DesignEntity::STMT:add_entity(*(pkb.getAllStatements()));
      break;
    case DesignEntity::READ:add_entity(*(pkb.getAllReadStatements()));
      break;
    case DesignEntity::PRINT:add_entity(*(pkb.getAllPrintStatements()));
      break;
    case DesignEntity::CALL:add_entity(*(pkb.getAllCallStatements()));
      break;
    case DesignEntity::WHILE:add_entity(*(pkb.getAllWhileStatements()));
      break;
    case DesignEntity::IF:add_entity(*(pkb.getAllIfStatements()));
      break;
    case DesignEntity::ASSIGN:add_entity(*(pkb.getAllAssignStatements()));
      break;
    case DesignEntity::VARIABLE:add_entity(*(pkb.getAllVariables()));
      break;
    case DesignEntity::CONSTANT:add_entity(*(pkb.getAllConstants()));
      break;
    case DesignEntity::PROCEDURE:add_entity(*(pkb.getAllProcedures()));
      break;
    default:throw std::invalid_argument("Invalid design entity in select statement");

  }
  return ConstructResult(entities);
}

} // qps
