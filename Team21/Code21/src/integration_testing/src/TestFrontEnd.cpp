#include <iostream>
#include <unordered_set>

#include "PKB/PKB.h"
#include "catch.hpp"
#include "sp/sp.h"

namespace test_frontend {
template <typename T, typename std::enable_if_t<std::is_base_of_v<Statement, T>,
                                                bool> = true>
void RequireStmtNumsMatch(const std::vector<T*>* stmts,
                          const std::unordered_set<int>& expectedStmtNums) {
  std::unordered_set<int> actualStmtNums;
  std::for_each(stmts->begin(), stmts->end(),
                [&actualStmtNums](Statement* stmt) {
                  actualStmtNums.insert(stmt->getStatementNumber());
                });

  REQUIRE(actualStmtNums == expectedStmtNums);
}

template <typename T,
          typename std::enable_if_t<std::is_base_of_v<Entity, T>, bool> = true>
void RequireEntityValuesMatch(
    const std::vector<T*>* entities,
    const std::unordered_set<std::string>& expectedEntityValues) {
  std::unordered_set<std::string> actualEntityValues;
  std::for_each(entities->begin(), entities->end(),
                [&actualEntityValues](Entity* stmt) {
                  actualEntityValues.insert(*stmt->getEntityValue());
                });

  REQUIRE(actualEntityValues == expectedEntityValues);
}

struct RelPair {
  RelationshipType relType;
  std::pair<EntityType, std::string> leftEntity;
  std::pair<EntityType, std::string> rightEntity;

  bool operator==(const RelPair& other) const {
    return relType == other.relType && leftEntity == other.leftEntity &&
           rightEntity == other.rightEntity;
  }

  friend std::ostream& operator<<(std::ostream& os, const RelPair& rel) {
    os << "RelPair{type=" << rel.relType.getKey() << ", left=(" << rel.leftEntity.first.getKey()
       << ", " << rel.leftEntity.second << "), right=(" << rel.rightEntity.first.getKey()
       << ", " << rel.rightEntity.second << ")}";
    return os;
  }

  struct Hasher {
    std::size_t operator()(const RelPair& rel) const {
      return std::hash<int>()(rel.relType.getKey()) ^
             (std::hash<int>()(rel.leftEntity.first.getKey()) ^
              (std::hash<std::string>()(rel.leftEntity.second) ^
               (std::hash<int>()(rel.rightEntity.first.getKey()) ^
                std::hash<std::string>()(rel.rightEntity.second))));
    }
  };
};

using RelPair = struct RelPair;

template <typename T, typename std::enable_if_t<
                          std::is_base_of_v<Relationship, T>, bool> = true>
void RequireRelationshipsMatch(
    const std::vector<T*>* rels,
    const std::unordered_set<RelPair, RelPair::Hasher>& expectedRels) {
  std::unordered_set<RelPair, RelPair::Hasher> actualRels;
  std::for_each(rels->begin(), rels->end(), [&actualRels](Relationship* rel) {
    RelPair relPair = {rel->getRelationshipType(),
                       {rel->getLeftHandEntity()->getEntityType(),
                        *rel->getLeftHandEntity()->getEntityValue()},
                       {rel->getRightHandEntity()->getEntityType(),
                        *rel->getRightHandEntity()->getEntityValue()}};
    actualRels.insert(relPair);
  });

  REQUIRE(actualRels == expectedRels);
}

SCENARIO("SP can process and store a simple program into PKB") {
  GIVEN("A simple program") {
    std::string program = R"(procedure main {
      x = 1;
      read y;
      print z;
      call foo;
    }
    procedure foo {
      z = 2;
      call bar;
    }
    procedure bar {
      x = 1;
      if (x == 1) then {
        call baz;
      } else {
        call qux;
      }
    }
    procedure baz {
      x = 4;
      call bar;
    }
    procedure qux {
      call foo;
    }
    procedure quux {
      call main;
    })";
    WHEN("The program is processed") {
      auto pkb = PKB();

      auto sp = sp::SP();
      sp.process(program, &pkb);

      QueryFacade* qf = pkb.getQueryFacade();
      THEN(
          "The PKB should contain the correct information about assign "
          "statements") {
        std::vector<AssignStatement*> const* assigns =
            qf->getAllAssignStatements();
        RequireStmtNumsMatch(assigns, {1, 5, 7, 11});
      }

      THEN(
          "The PKB should contain the correct information about call "
          "statements") {
        std::vector<CallStatement*> const* calls = qf->getAllCallStatements();
        RequireStmtNumsMatch(calls, {4, 6, 9, 10, 12, 13, 14});
      }

      THEN(
          "The PKB should contain the correct information about if "
          "statements") {
        std::vector<IfStatement*> const* ifs = qf->getAllIfStatements();
        RequireStmtNumsMatch(ifs, {8});
      }

      THEN(
          "The PKB should contain the correct information about while "
          "statements") {
        std::vector<WhileStatement*> const* whiles =
            qf->getAllWhileStatements();
        RequireStmtNumsMatch(whiles, {});
      }

      THEN(
          "The PKB should contain the correct information about print "
          "statements") {
        std::vector<PrintStatement*> const* prints =
            qf->getAllPrintStatements();
        RequireStmtNumsMatch(prints, {3});
      }

      THEN(
          "The PKB should contain the correct information about read "
          "statements") {
        std::vector<ReadStatement*> const* reads = qf->getAllReadStatements();
        RequireStmtNumsMatch(reads, {2});
      }

      THEN("The PKB should contain the correct information about procedures") {
        std::vector<Procedure*> const* procs = qf->getAllProcedures();
        RequireEntityValuesMatch(procs,
                                 {"main", "foo", "bar", "baz", "qux", "quux"});
      }

      THEN("The PKB should contain the correct information about variables") {
        std::vector<Variable*> const* vars = qf->getAllVariables();
        RequireEntityValuesMatch(vars, {"x", "y", "z"});
      }

      THEN("The PKB should contain the correct information about constants") {
        std::vector<Constant*> const* consts = qf->getAllConstants();
        RequireEntityValuesMatch(consts, {"1", "2", "4"});
      }

      THEN("The PKB should contain the correct information about statements") {
        std::vector<Statement*> const* stmts = qf->getAllStatements();
        RequireStmtNumsMatch(stmts,
                             {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
      }

      THEN("The PKB should contain all Calls relationships") {
        std::vector<CallsRelationship*> const* callsRels =
            qf->getAllCallsRelationships();

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{CallsRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Procedure::getEntityTypeStatic(), "foo"}},
            RelPair{CallsRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Procedure::getEntityTypeStatic(), "bar"}},
            RelPair{CallsRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Procedure::getEntityTypeStatic(), "baz"}},
            RelPair{CallsRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Procedure::getEntityTypeStatic(), "qux"}},
            RelPair{CallsRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Procedure::getEntityTypeStatic(), "bar"}},
            RelPair{CallsRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Procedure::getEntityTypeStatic(), "foo"}},
            RelPair{CallsRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Procedure::getEntityTypeStatic(), "main"}}};

        RequireRelationshipsMatch(callsRels, expectedRels);
      }

      THEN("The PKB should contain all Calls* relationships") {
        std::vector<CallsStarRelationship*> const* callsStarRels =
            qf->getAllCallsStarRelationships();

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Procedure::getEntityTypeStatic(), "foo"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Procedure::getEntityTypeStatic(), "bar"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Procedure::getEntityTypeStatic(), "baz"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Procedure::getEntityTypeStatic(), "qux"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Procedure::getEntityTypeStatic(), "bar"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Procedure::getEntityTypeStatic(), "baz"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Procedure::getEntityTypeStatic(), "qux"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Procedure::getEntityTypeStatic(), "foo"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Procedure::getEntityTypeStatic(), "baz"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Procedure::getEntityTypeStatic(), "qux"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Procedure::getEntityTypeStatic(), "foo"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Procedure::getEntityTypeStatic(), "bar"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Procedure::getEntityTypeStatic(), "bar"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Procedure::getEntityTypeStatic(), "baz"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Procedure::getEntityTypeStatic(), "qux"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Procedure::getEntityTypeStatic(), "foo"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Procedure::getEntityTypeStatic(), "foo"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Procedure::getEntityTypeStatic(), "bar"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Procedure::getEntityTypeStatic(), "baz"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Procedure::getEntityTypeStatic(), "qux"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Procedure::getEntityTypeStatic(), "main"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Procedure::getEntityTypeStatic(), "foo"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Procedure::getEntityTypeStatic(), "bar"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Procedure::getEntityTypeStatic(), "baz"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Procedure::getEntityTypeStatic(), "qux"}}};

        RequireRelationshipsMatch(callsStarRels, expectedRels);
      }

      THEN("The PKB should contain all proc-var Modifies relationships") {
        std::vector<ModifiesRelationship*> const* modifiesRels =
            qf->getModifiesRelationshipsByLeftAndRightEntityTypes(
                Procedure::getEntityTypeStatic(), Variable::getEntityTypeStatic());

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Variable::getEntityTypeStatic(), "y"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Variable::getEntityTypeStatic(), "y"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Variable::getEntityTypeStatic(), "z"}}};

        RequireRelationshipsMatch(modifiesRels, expectedRels);
      }

      THEN("The PKB should contain all stmt-var Modifies relationships") {
        std::vector<ModifiesRelationship*> const* modifiesRels =
            qf->getModifiesRelationshipsByLeftAndRightEntityTypes(
                Statement::getEntityTypeStatic(), Variable::getEntityTypeStatic());

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {AssignStatement::getEntityTypeStatic(), "1"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {ReadStatement::getEntityTypeStatic(), "2"},
                    {Variable::getEntityTypeStatic(), "y"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "4"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "4"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {AssignStatement::getEntityTypeStatic(), "5"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "6"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "6"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {AssignStatement::getEntityTypeStatic(), "7"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {IfStatement::getEntityTypeStatic(), "8"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {IfStatement::getEntityTypeStatic(), "8"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "9"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "9"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "10"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "10"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {AssignStatement::getEntityTypeStatic(), "11"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "12"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "12"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "14"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "14"},
                    {Variable::getEntityTypeStatic(), "y"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "14"},
                    {Variable::getEntityTypeStatic(), "z"}}};

        RequireRelationshipsMatch(modifiesRels, expectedRels);
      }

      THEN("The PKB should contain all proc-var Uses relationships") {
        std::vector<UsesRelationship*> const* usesRels =
            qf->getUsesRelationshipsByLeftAndRightEntityTypes(
                Procedure::getEntityTypeStatic(), Variable::getEntityTypeStatic());

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Variable::getEntityTypeStatic(), "z"}}};

        RequireRelationshipsMatch(usesRels, expectedRels);
      }

      THEN("The PKB should contain all stmt-var Uses relationships") {
        std::vector<UsesRelationship*> const* usesRels =
            qf->getUsesRelationshipsByLeftAndRightEntityTypes(
                Statement::getEntityTypeStatic(), Variable::getEntityTypeStatic());

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {PrintStatement::getEntityTypeStatic(), "3"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "4"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "6"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {IfStatement::getEntityTypeStatic(), "8"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "9"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "10"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "12"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "14"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "14"},
                    {Variable::getEntityTypeStatic(), "z"}}};

        RequireRelationshipsMatch(usesRels, expectedRels);
      }
    }
  }
}
}  // namespace test_frontend
