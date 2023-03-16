#include "select_evaluator.h"
#include "query_evaluators/join/constraints_solver.h"
#include "attr_ref_eval.h"

namespace qps {

class SelectVisitor {
 public:
  SelectVisitor(QueryFacade &pkb, std::vector<Declaration> &declarations) : pkb_(pkb), declarations_(declarations) {}
  FinalResult operator()(Boolean &&, bool &&current_result) {
    return current_result;
  }
  FinalResult operator()(Boolean &&, SynonymTable &&current_table) {
    return !current_table.IsEmpty();
  }
  FinalResult operator()(std::vector<Element> &&elements, bool &&current_result) {
    if (!current_result) return FinalTable{};
    UnpackElements(elements);
    EvaluateSelectSyns();
    return EvaluateFinalResult();
  }
  FinalResult operator()(std::vector<Element> &&elements, SynonymTable &&current_table) {
    if (current_table.IsEmpty()) return FinalTable{};
    UnpackElements(elements);
    EvaluateSelectSyns(current_table);
    tables_.push_back(std::move(current_table));
    return EvaluateFinalResult();
  }

 private:
  FinalResult EvaluateFinalResult() {
    auto final_syn_table = ConstraintsSolver::solve(tables_);
    FinalTable final_table{{syns_}};
    for (size_t row_idx = 0; row_idx < final_syn_table.ResultSize(); ++row_idx) {
      FinalTable::Row row;
      size_t attr_refs_idx = 0;
      for (size_t col_idx = 0; col_idx < syns_.size(); ++col_idx) {
        auto &syn = syns_[col_idx];
        auto val = final_syn_table.GetCell(row_idx, syn);
        if (attr_refs_idx < attr_refs_.size() && attr_refs_[attr_refs_idx].synonym == syn) {
          row.push_back(EvaluateAttrRef(val,
                                        attr_refs_[attr_refs_idx].attrName, pkb_));
          attr_refs_idx++;
        } else {
          row.push_back(*val->getEntityValue());
        }
      }
      final_table.AddRow(row);
    }
    return final_table;
  }

  void UnpackElements(const std::vector<Element> &elements) {
    for (const auto &element : elements) {
      if (std::holds_alternative<AttrRef>(element)) {
        auto attr_ref = std::get<AttrRef>(element);
        syns_.push_back(attr_ref.synonym);
        attr_refs_.push_back(std::move(attr_ref));
      } else {
        syns_.push_back(std::get<Synonym>(element));
      }
    }
  }

  void EvaluateSelectSyns() {
    for (auto &syn : syns_) {
      EvaluateSingleSelect(syn);
    }
  }

  void EvaluateSelectSyns(const SynonymTable &current_table) {
    for (auto &syn : syns_) {
      if (current_table.HasKey(syn)) continue;
      EvaluateSingleSelect(syn);
    }
  }

  void EvaluateSingleSelect(Synonym &syn) {
    auto design_entity = Declaration::findDeclarationWithSynonym(declarations_, syn);
    if (!design_entity) return;
    auto entity_type = ClauseEvaluator::DesignEntityToEntityType(design_entity->getDesignEntity());
    auto entities = pkb_.getEntitiesByType(entity_type);
    std::vector<std::vector<Entity *>> rows;
    rows.reserve(entities->size());
    for (const auto &entity : *entities) {
      rows.push_back({entity});
    }
    tables_.push_back(SynonymTable{{syn}, rows});

  }

  QueryFacade &pkb_;
  std::vector<Declaration> &declarations_;
  std::vector<AttrRef> attr_refs_;
  std::vector<Synonym> syns_;
  std::vector<SynonymTable> tables_;
};

FinalResult SelectEvaluator::Evaluate(QueryFacade &pkb, ClauseResult current_result) {
  SelectVisitor visitor{pkb, declarations_};
  return std::visit(visitor, std::move(result_), std::move(current_result));
}

} // qps
