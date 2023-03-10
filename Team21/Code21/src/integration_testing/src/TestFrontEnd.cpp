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
    os << "RelPair{type=" << rel.relType << ", left=(" << rel.leftEntity.first
       << ", " << rel.leftEntity.second << "), right=(" << rel.rightEntity.first
       << ", " << rel.rightEntity.second << ")}";
    return os;
  }

  struct Hasher {
    std::size_t operator()(const RelPair& rel) const {
      return std::hash<int>()(rel.relType) ^
             (std::hash<int>()(rel.leftEntity.first) ^
              (std::hash<std::string>()(rel.leftEntity.second) ^
               (std::hash<int>()(rel.rightEntity.first) ^
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
    }
    procedure qux {
      print w;
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
        RequireStmtNumsMatch(calls, {4, 6, 9, 10, 13});
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
        RequireStmtNumsMatch(prints, {3, 12});
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
        RequireEntityValuesMatch(vars, {"w", "x", "y", "z"});
      }

      THEN("The PKB should contain the correct information about constants") {
        std::vector<Constant*> const* consts = qf->getAllConstants();
        RequireEntityValuesMatch(consts, {"1", "2", "4"});
      }

      THEN("The PKB should contain the correct information about statements") {
        std::vector<Statement*> const* stmts = qf->getAllStatements();
        RequireStmtNumsMatch(stmts,
                             {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
      }

      THEN("The PKB should contain all Calls relationships") {
        std::vector<CallsRelationship*> const* callsRels =
            qf->getAllCallsRelationships();

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{RelationshipType::CALLS,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::PROCEDURE, "foo"}},
            RelPair{RelationshipType::CALLS,
                    {EntityType::PROCEDURE, "foo"},
                    {EntityType::PROCEDURE, "bar"}},
            RelPair{RelationshipType::CALLS,
                    {EntityType::PROCEDURE, "bar"},
                    {EntityType::PROCEDURE, "baz"}},
            RelPair{RelationshipType::CALLS,
                    {EntityType::PROCEDURE, "bar"},
                    {EntityType::PROCEDURE, "qux"}},
            RelPair{RelationshipType::CALLS,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::PROCEDURE, "main"}}};

        RequireRelationshipsMatch(callsRels, expectedRels);
      }

      THEN("The PKB should contain all Calls* relationships") {
        std::vector<CallsStarRelationship*> const* callsStarRels =
            qf->getAllCallsStarRelationships();

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::PROCEDURE, "foo"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::PROCEDURE, "bar"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::PROCEDURE, "baz"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::PROCEDURE, "qux"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "foo"},
                    {EntityType::PROCEDURE, "bar"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "foo"},
                    {EntityType::PROCEDURE, "baz"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "foo"},
                    {EntityType::PROCEDURE, "qux"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "bar"},
                    {EntityType::PROCEDURE, "baz"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "bar"},
                    {EntityType::PROCEDURE, "qux"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::PROCEDURE, "main"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::PROCEDURE, "foo"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::PROCEDURE, "bar"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::PROCEDURE, "baz"}},
            RelPair{RelationshipType::CALLS_STAR,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::PROCEDURE, "qux"}}};

        RequireRelationshipsMatch(callsStarRels, expectedRels);
      }

      THEN("The PKB should contain all proc-var Modifies relationships") {
        std::vector<ModifiesRelationship*> const* modifiesRels =
            qf->getModifiesRelationshipsByLeftAndRightEntityTypes(
                EntityType::PROCEDURE, EntityType::VARIABLE);

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::VARIABLE, "y"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::VARIABLE, "z"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "foo"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "foo"},
                    {EntityType::VARIABLE, "z"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "bar"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "baz"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::VARIABLE, "y"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::VARIABLE, "z"}}};

        RequireRelationshipsMatch(modifiesRels, expectedRels);
      }

      THEN("The PKB should contain all stmt-var Modifies relationships") {
        std::vector<ModifiesRelationship*> const* modifiesRels =
            qf->getModifiesRelationshipsByLeftAndRightEntityTypes(
                EntityType::STATEMENT, EntityType::VARIABLE);

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::ASSIGN_STATEMENT, "1"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::READ_STATEMENT, "2"},
                    {EntityType::VARIABLE, "y"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::CALL_STATEMENT, "4"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::CALL_STATEMENT, "4"},
                    {EntityType::VARIABLE, "z"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::ASSIGN_STATEMENT, "5"},
                    {EntityType::VARIABLE, "z"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::CALL_STATEMENT, "6"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::ASSIGN_STATEMENT, "7"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::IF_STATEMENT, "8"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::CALL_STATEMENT, "9"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::ASSIGN_STATEMENT, "11"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::CALL_STATEMENT, "13"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::CALL_STATEMENT, "13"},
                    {EntityType::VARIABLE, "y"}},
            RelPair{RelationshipType::MODIFIES,
                    {EntityType::CALL_STATEMENT, "13"},
                    {EntityType::VARIABLE, "z"}}};

        RequireRelationshipsMatch(modifiesRels, expectedRels);
      }

      THEN("The PKB should contain all proc-var Uses relationships") {
        std::vector<UsesRelationship*> const* usesRels =
            qf->getUsesRelationshipsByLeftAndRightEntityTypes(
                EntityType::PROCEDURE, EntityType::VARIABLE);

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "main"},
                    {EntityType::VARIABLE, "z"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "foo"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "foo"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "bar"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "bar"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "qux"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PROCEDURE, "quux"},
                    {EntityType::VARIABLE, "z"}}};

        RequireRelationshipsMatch(usesRels, expectedRels);
      }

      THEN("The PKB should contain all stmt-var Uses relationships") {
        std::vector<UsesRelationship*> const* usesRels =
            qf->getUsesRelationshipsByLeftAndRightEntityTypes(
                EntityType::STATEMENT, EntityType::VARIABLE);

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{RelationshipType::USES,
                    {EntityType::PRINT_STATEMENT, "3"},
                    {EntityType::VARIABLE, "z"}},
            RelPair{RelationshipType::USES,
                    {EntityType::CALL_STATEMENT, "4"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::CALL_STATEMENT, "4"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::USES,
                    {EntityType::CALL_STATEMENT, "6"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::CALL_STATEMENT, "6"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::USES,
                    {EntityType::IF_STATEMENT, "8"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::IF_STATEMENT, "8"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::USES,
                    {EntityType::CALL_STATEMENT, "10"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::PRINT_STATEMENT, "12"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::CALL_STATEMENT, "13"},
                    {EntityType::VARIABLE, "w"}},
            RelPair{RelationshipType::USES,
                    {EntityType::CALL_STATEMENT, "13"},
                    {EntityType::VARIABLE, "x"}},
            RelPair{RelationshipType::USES,
                    {EntityType::CALL_STATEMENT, "13"},
                    {EntityType::VARIABLE, "z"}}};

        RequireRelationshipsMatch(usesRels, expectedRels);
      }
    }
  }
}
}  // namespace test_frontend
