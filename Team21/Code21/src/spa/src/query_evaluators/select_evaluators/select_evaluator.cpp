#include "select_evaluator.h"
#include "query_evaluators/join/constraints_solver.h"
#include "attr_ref_eval.h"
#include "query_evaluators/pkb_qps_type_mapping.h"
#include "query_evaluators/string_helpers.h"

namespace qps {

class SelectVisitor {
 public:
  SelectVisitor(QueryFacade &pkb, std::vector<Declaration> &declarations, std::list<std::string> &results)
      : pkb_(pkb), declarations_(declarations), results_(results) {}

//  void operator()([[maybe_unused]] Boolean &&boolean, bool &&current_result) {
//    results_.emplace_back(to_string(current_result));
//  }
  template<typename ClauseResult>
  void operator()([[maybe_unused]] Boolean &&boolean, ClauseResult current_result) {
    results_.emplace_back(to_string(static_cast<bool>(current_result)));
  }
  template<typename ClauseResult>
  void operator()(std::vector<Element> &&elements, ClauseResult current_result) {
    if (!current_result) {
      return;
    }
    UnpackElements(elements);
    EvaluateSelectSyns(std::move(current_result));
    EvaluateFinalResult();
  }

 private:
  void EvaluateFinalResult() {
    auto final_result = ConstraintsSolver::solve(tables_);
    if (!final_result) {
      results_.emplace_back(to_string(false));
      return;
    }
    auto final_table = std::move(final_result.value());
    for (size_t row_idx = 0; row_idx < final_table.ResultSize(); ++row_idx) {
      std::vector<std::string> row;
      for (size_t col_idx = 0; col_idx < syns_.size(); ++col_idx) {
        auto &syn = syns_[col_idx];
        auto *val = final_table.GetCell(row_idx, syn);
        auto attr_name = attr_refs_.find(col_idx);
        if (attr_name != attr_refs_.end()) {
          row.push_back(AttributeReferenceEvaluator::EvaluateAttrRef(val,
                                                                     attr_name->second, pkb_));
        } else {
          row.push_back(*val->getEntityValue());
        }
      }
      results_.push_back(Join(std::move(row), " "));
    }
  }

  void UnpackElements(const std::vector<Element> &elements) {
    for (size_t i = 0; i < elements.size(); ++i) {
      const auto &element = elements[i];
      if (std::holds_alternative<AttrRef>(element)) {
        auto attr_ref = std::get<AttrRef>(element);
        syns_.push_back(attr_ref.synonym);
        attr_refs_[i] = attr_ref.attrName;
      } else {
        syns_.push_back(std::get<Synonym>(element));
      }
    }
  }

  void EvaluateSelectSyns(bool current_result) {
    for (auto &syn : syns_) {
      EvaluateSingleSelect(syn);
    }
  }

  void EvaluateSelectSyns(SynonymTable current_table) {
    for (auto &syn : syns_) {
      if (current_table.HasKey(syn)) {
        continue;
      }
      EvaluateSingleSelect(syn);
    }
    tables_.push_back(std::move(current_table));
  }

  void EvaluateSingleSelect(Synonym &syn) {
    auto design_entity = Declaration::findDeclarationWithSynonym(declarations_, syn);
    if (!design_entity) {
      return;
    }
    auto entity_type = DesignEntityToEntityType(design_entity->getDesignEntity());
    auto *entities = pkb_.getEntitiesByType(entity_type);
    std::vector<std::vector<Entity *>> rows;
    rows.reserve(entities->size());
    for (const auto &entity : *entities) {
      rows.push_back({entity});
    }
    tables_.push_back(SynonymTable{{syn}, rows});

  }

  QueryFacade &pkb_;
  std::vector<Declaration> &declarations_;
  std::unordered_map<size_t, AttrName> attr_refs_;
  std::vector<Synonym> syns_;
  std::vector<SynonymTable> tables_;
  std::list<std::string> &results_;
};

void SelectEvaluator::Evaluate(QueryFacade &pkb, ClauseResult current_result, std::list<std::string> &results) {
  SelectVisitor visitor{pkb, declarations_, results};
  std::visit(visitor, std::move(result_), std::move(current_result));
}

} // namespace qps
