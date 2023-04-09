#include <unordered_set>

#include "PKB/PKB.h"
#include "query/design_entity.h"

namespace qps_test {
class PopulatePKBHelper {

 public:
  using Data = std::unordered_map<qps::DesignEntity, std::unordered_set<std::string>>;

  PopulatePKBHelper() {
    pkb_ = PKB();
  }

  auto GetQuerier() -> QueryFacade * {
    return pkb_.getQueryFacade();
  }

  auto GetPopulater() -> PopulateFacade * {
    return pkb_.getPopulateFacade();
  }

  void PopulateEntities(Data &data) {
    AddVariables(data[qps::DesignEntity::VARIABLE]);
    AddConstants(data[qps::DesignEntity::CONSTANT]);
    AddAssignments(data[qps::DesignEntity::ASSIGN]);
    AddRead(data[qps::DesignEntity::READ]);
    AddPrint(data[qps::DesignEntity::PRINT]);
    AddIf(data[qps::DesignEntity::IF]);
    AddWhile(data[qps::DesignEntity::WHILE]);
    AddCalls(data[qps::DesignEntity::CALL]);
    AddProcedures(data[qps::DesignEntity::PROCEDURE]);

  }

  void StoreCFG(std::shared_ptr<cfg::CFG> cfg) {
    pkb_.getPopulateFacade()->storeCFG(std::move(cfg));
  }

  void AddPostFixExpressions(const std::vector<std::pair<int, std::string>> &assignments) {
    for (const auto &[stmt, postfix] : assignments) {
      pkb_.getPopulateFacade()->storeAssignStatementPostfixExpression(stmt, postfix);
    }
  }
  void AddWhileStatementConditionVariable(const std::vector<std::pair<int, std::string>> &whiles) {
    for (const auto &[stmt, var_name] : whiles) {
        pkb_.getPopulateFacade()->storeUsesInParentConditionRelationship(stmt, var_name);
    }
  }

  void AddIfStatementConditionVariable(const std::vector<std::pair<int, std::string>> &ifs) {
    for (const auto &[stmt, var_name] : ifs) {
        pkb_.getPopulateFacade()->storeUsesInParentConditionRelationship(stmt, var_name);
    }
  }

  void AddStatementModifies(const std::vector<std::pair<int, std::string>> &modifies) {
    for (const auto &[stmt, var_name] : modifies) {
      pkb_.getPopulateFacade()->storeStatementModifiesVariableRelationship(stmt, var_name);
    }
  }

  void AddProcedureModifies(const std::vector<std::pair<std::string, std::string>> &modifies) {
    for (const auto &[proc_name, var_name] : modifies) {
      pkb_.getPopulateFacade()->storeProcedureModifiesVariableRelationship(proc_name, var_name);
    }
  }

  void AddProcedureUses(const std::vector<std::pair<std::string, std::string>> &uses) {
    for (const auto &[proc_name, var_name] : uses) {
      pkb_.getPopulateFacade()->storeProcedureUsesVariableRelationship(proc_name, var_name);
    }
  }

  void AddStatementUses(const std::vector<std::pair<int, std::string>> &uses) {
    for (const auto &[stmt, var_name] : uses) {
      pkb_.getPopulateFacade()->storeStatementUsesVariableRelationship(stmt, var_name);
    }
  }

  void AddParent(const std::vector<std::pair<int, int>> &parent) {
    for (const auto &[parent, child] : parent) {
      pkb_.getPopulateFacade()->storeParentRelationship(parent, child);
    }
  }

  void AddParentStar(const std::vector<std::pair<int, int>> &parent) {
    for (const auto &[parent, child] : parent) {
      pkb_.getPopulateFacade()->storeParentStarRelationship(parent, child);
    }
  }

  void AddFollows(const std::vector<std::pair<int, int>> &follows) {
    for (const auto &[before, after] : follows) {
      pkb_.getPopulateFacade()->storeFollowsRelationship(before, after);
    }
  }

  void AddFollowsStar(const std::vector<std::pair<int, int>> &follows) {
    for (const auto &[parent, child] : follows) {
      pkb_.getPopulateFacade()->storeFollowsStarRelationship(parent, child);
    }
  }

  void AddCallsR(const std::vector<std::pair<std::string, std::string>> &calls) {
    for (const auto &[proc1, proc2] : calls) {
      pkb_.getPopulateFacade()->storeCallsRelationship(proc1, proc2);
    }
  }

  void AddCallsStar(const std::vector<std::pair<std::string, std::string>> &calls) {
    for (const auto &[proc1, proc2] : calls) {
      pkb_.getPopulateFacade()->storeCallsStarRelationship(proc1, proc2);
    }
  }

  void AddVariables(const std::unordered_set<std::string> &variables) {
    for (const auto &str : variables) {
      pkb_.getPopulateFacade()->storeVariable(str);
    }
  }

  void AddConstants(const std::unordered_set<std::string> &constants) {
    for (const auto &str : constants) {
      pkb_.getPopulateFacade()->storeConstant(std::stoi(str));
    }
  }

  void AddAssignments(const std::unordered_set<std::string> &assignments) {
    for (const auto &str : assignments) {
      pkb_.getPopulateFacade()->storeAssignmentStatement(std::stoi(str));
    }
  }

  void AddCalls(const std::unordered_set<std::string> &calls) {
    for (const auto &str : calls) {
      pkb_.getPopulateFacade()->storeCallStatement(std::stoi(str));
    }
  }

  void AddProcedures(const std::unordered_set<std::string> &procs) {
    for (const auto &str : procs) {
      pkb_.getPopulateFacade()->storeProcedure(str);
    }
  }
  void AddPrint(const std::unordered_set<std::string> &prints) {
    for (const auto &str : prints) {
      pkb_.getPopulateFacade()->storePrintStatement(std::stoi(str));
    }
  }
  void AddRead(const std::unordered_set<std::string> &reads) {
    for (const auto &str : reads) {
      pkb_.getPopulateFacade()->storeReadStatement(std::stoi(str));
    }
  }

  void AddIf(const std::unordered_set<std::string> &ifs) {
    for (const auto &str : ifs) {
      pkb_.getPopulateFacade()->storeIfStatement(std::stoi(str));
    }
  }

  void AddWhile(const std::unordered_set<std::string> &whiles) {
    for (const auto &str : whiles) {
      pkb_.getPopulateFacade()->storeWhileStatement(std::stoi(str));
    }
  }

 private:
  PKB pkb_;
};
}  // namespace qps_test
