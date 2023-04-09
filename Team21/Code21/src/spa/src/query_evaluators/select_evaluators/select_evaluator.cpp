#include "select_evaluator.h"
#include "query_evaluators/join/constraints_solver.h"
#include "attr_ref_eval.h"
#include "query_evaluators/pkb_qps_type_mapping.h"
#include "query_evaluators/string_helpers.h"
#include "query_evaluators/tables/table_helpers.h"

namespace qps {

class SelectVisitor {
 public:
  SelectVisitor(QueryFacade &pkb, std::vector<Declaration> &declarations, std::list<std::string> &results)
      : pkb_(pkb), declarations_(declarations), results_(results) {}

  template<typename ClauseResult>
  void operator()([[maybe_unused]] Boolean &&boolean, ClauseResult current_result) {
    results_.emplace_back(to_string(GetBooleanValue(current_result)));
  }

  template<typename ClauseResult>
  void operator()(std::vector<Element> &&elements, ClauseResult current_result) {
    if (!GetBooleanValue(current_result)) {
      return;
    }
    UnpackElements(elements);
    EvaluateSelectSyns(std::move(current_result));
    EvaluateFinalResult();
  }

 private:
  static auto GetBooleanValue(const ClauseResults &result) -> bool {
    if (std::holds_alternative<bool>(result)) {
      return std::get<bool>(result);
    }
    return !std::get<ClauseGrps>(result).grp_table.empty();
  }

  void EvaluateFinalResult() {
    auto final_result = ConstraintsSolver::solve(std::move(tables_));
    if (!final_result) {
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
      results_.push_back(Join(row, " "));
    }
  }

  void UnpackElements(const std::vector<Element> &elements) {
    for (size_t i = 0; i < elements.size(); ++i) {
      const auto &element = elements[i];
      if (std::holds_alternative<AttrRef>(element)) {
        auto attr_ref = std::get<AttrRef>(element);
        syns_.push_back(attr_ref.synonym);
        seen_syns_.insert(attr_ref.synonym);
        attr_refs_[i] = attr_ref.attrName;
      } else {
        syns_.push_back(std::get<Synonym>(element));
        seen_syns_.insert(std::get<Synonym>(element));
      }
    }
  }

  void EvaluateSelectSyns(ClauseGrps clause_grps) {
    std::unordered_map<int, std::vector<Synonym>> filtered_grps;
    for (auto &syn : syns_) {
      auto grp = clause_grps.syn_grp.find(syn);
      if (grp == clause_grps.syn_grp.end()) {
        EvaluateSingleSelect(syn);
      } else {
        filtered_grps[grp->second].push_back(syn);
      }
    }
    for (const auto &iter : filtered_grps) {
      auto projected_table = Project(clause_grps.grp_table[iter.first], iter.second);
      tables_.push_back(std::move(projected_table));
    }
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

  void EvaluateSelectSyns(bool current_result) {
    for (auto &syn : syns_) {
      EvaluateSingleSelect(syn);
    }
  }

  QueryFacade &pkb_;
  std::vector<Declaration> &declarations_;
  std::list<std::string> &results_;

  std::unordered_map<size_t, AttrName> attr_refs_;
  std::unordered_set<Synonym> seen_syns_;
  std::vector<Synonym> syns_;
  std::vector<SynonymTable> tables_;
};

void SelectEvaluator::Evaluate(QueryFacade &pkb, ClauseResults current_result, std::list<std::string> &results) {
  SelectVisitor visitor{pkb, declarations_, results};
  std::visit(visitor, std::move(result_), std::move(current_result));
}

} // namespace qps
