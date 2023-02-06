#include <unordered_set>

#include "PKB/PKB.h"

class PopulatePKBHelper {

 public:
  PopulatePKBHelper() {
    pkb_ = new PKB();
  }

  ~PopulatePKBHelper() {
    delete pkb_;
  }

  QueryFacade *GetQuerier() {
    return pkb_->getQueryFacade();
  }

  PopulateFacade *GetPopulater() {
    return pkb_->getPopulateFacade();
  }
  void AddVariables(const std::unordered_set<std::string> &variables) {
    for (const auto &str : variables) {
      pkb_->getPopulateFacade()->storeVariable(str);
    }
  }

  void AddConstants(const std::unordered_set<std::string> &constants) {
    for (const auto &str : constants) {
      pkb_->getPopulateFacade()->storeConstant(std::stoi(str));
    }
  }

  void AddAssignments(const std::unordered_set<std::string> &assignments) {
    for (const auto &str : assignments) {
      pkb_->getPopulateFacade()->storeAssignmentStatement(std::stoi(str));
    }
  }

  void AddCalls(const std::unordered_set<std::string> &calls) {
    for (const auto &str : calls) {
      pkb_->getPopulateFacade()->storeCallStatement(std::stoi(str));
    }
  }

  void AddProcedures(const std::unordered_set<std::string> &procs) {
    for (const auto &str : procs) {
      pkb_->getPopulateFacade()->storeProcedure(str);
    }
  }
  void AddPrint(const std::unordered_set<std::string> &prints) {
    for (const auto &str : prints) {
      pkb_->getPopulateFacade()->storePrintStatement(std::stoi(str));
    }
  }
  void AddRead(const std::unordered_set<std::string> &reads) {
    for (const auto &str : reads) {
      pkb_->getPopulateFacade()->storeReadStatement(std::stoi(str));
    }
  }

  void AddIf(const std::unordered_set<std::string> &ifs) {
    for (const auto &str : ifs) {
      pkb_->getPopulateFacade()->storeIfStatement(std::stoi(str));
    }
  }

  void AddWhile(const std::unordered_set<std::string> &whiles) {
    for (const auto &str : whiles) {
      pkb_->getPopulateFacade()->storeWhileStatement(std::stoi(str));
    }
  }

 private:
  PKB *pkb_;
};

