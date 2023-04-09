#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <unordered_set>

#include "PKB/PKB.h"
#include "sp/sp.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

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

  auto operator==(const RelPair& other) const -> bool {
    return relType == other.relType && leftEntity == other.leftEntity &&
           rightEntity == other.rightEntity;
  }

  friend auto operator<<(std::ostream& ostream, const RelPair& rel) -> std::ostream& {
    ostream << "RelPair{type=" << rel.relType.getKey() << ", left=(" << rel.leftEntity.first.getKey()
            << ", " << rel.leftEntity.second << "), right=(" << rel.rightEntity.first.getKey()
            << ", " << rel.rightEntity.second << ")}";
    return ostream;
  }

  struct Hasher {
    auto operator()(const RelPair& rel) const -> std::size_t {
      return std::hash<size_t>()(rel.relType.getKey()) ^
             (std::hash<size_t>()(rel.leftEntity.first.getKey()) ^
              (std::hash<std::string>()(rel.leftEntity.second) ^
               (std::hash<size_t>()(rel.rightEntity.first.getKey()) ^
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

      sp::SP::process(program, &pkb);

      QueryFacade* queryFacade = pkb.getQueryFacade();
      THEN(
          "The PKB should contain the correct information about assign "
          "statements") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<AssignStatement*> const* assigns =
            reinterpret_cast<std::vector<AssignStatement*> const*>(queryFacade->getEntitiesByType(AssignStatement::getEntityTypeStatic()));
        RequireStmtNumsMatch(assigns, {1, 5, 7, 11});
      }

      THEN(
          "The PKB should contain the correct information about call "
          "statements") {
        std::vector<CallStatement*> const* calls = reinterpret_cast<std::vector<CallStatement*> const*>(queryFacade->getEntitiesByType(CallStatement::getEntityTypeStatic()));
        RequireStmtNumsMatch(calls, {4, 6, 9, 10, 13});
      }

      THEN(
          "The PKB should contain the correct information about if "
          "statements") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<IfStatement*> const* ifs = reinterpret_cast<std::vector<IfStatement*> const*>(queryFacade->getEntitiesByType(IfStatement::getEntityTypeStatic()));

        RequireStmtNumsMatch(ifs, {8});
      }

      THEN(
          "The PKB should contain the correct information about while "
          "statements") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<WhileStatement*> const* whiles = reinterpret_cast<std::vector<WhileStatement*> const*>(queryFacade->getEntitiesByType(WhileStatement::getEntityTypeStatic()));
        RequireStmtNumsMatch(whiles, {});
      }

      THEN(
          "The PKB should contain the correct information about print "
          "statements") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)

        std::vector<PrintStatement*> const* prints =
            reinterpret_cast<std::vector<PrintStatement*> const*>(queryFacade->getEntitiesByType(PrintStatement::getEntityTypeStatic()));
        RequireStmtNumsMatch(prints, {3, 12});
      }

      THEN(
          "The PKB should contain the correct information about read "
          "statements") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)

        std::vector<ReadStatement*> const* reads = reinterpret_cast<std::vector<ReadStatement*> const*>(queryFacade->getEntitiesByType(
            ReadStatement::getEntityTypeStatic()));
        RequireStmtNumsMatch(reads, {2});
      }

      THEN("The PKB should contain the correct information about procedures") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<Procedure*> const* procs = reinterpret_cast<std::vector<Procedure*> const*>(
            queryFacade->getEntitiesByType(Procedure::getEntityTypeStatic()));
        RequireEntityValuesMatch(procs,
                                 {"main", "foo", "bar", "baz", "qux", "quux"});
      }

      THEN("The PKB should contain the correct information about variables") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<Variable*> const* vars = reinterpret_cast<std::vector<Variable*> const*>(
            queryFacade->getEntitiesByType(Variable::getEntityTypeStatic()));
        RequireEntityValuesMatch(vars, {"w", "x", "y", "z"});
      }

      THEN("The PKB should contain the correct information about constants") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<Constant*> const* consts = reinterpret_cast<std::vector<Constant*> const*>(
            queryFacade->getEntitiesByType(Constant::getEntityTypeStatic()));
        RequireEntityValuesMatch(consts, {"1", "2", "4"});
      }

      THEN("The PKB should contain the correct information about statements") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<Statement*> const* stmts =
            reinterpret_cast<std::vector<Statement*> const*>(
                queryFacade->getEntitiesByType(Statement::getEntityTypeStatic()));
        RequireStmtNumsMatch(stmts,
                             {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13});
      }

      THEN("The PKB should contain all Calls relationships") {
        auto* callsRels = queryFacade->getRelationshipsByTypes(CallsRelationship::getRelationshipTypeStatic(), Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic());

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
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Procedure::getEntityTypeStatic(), "main"}}};

        RequireRelationshipsMatch(callsRels, expectedRels);
      }

      THEN("The PKB should contain all Calls* relationships") {
        auto* callsStarRels = (queryFacade->getRelationshipsByTypes(CallsStarRelationship::getRelationshipTypeStatic(), Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic()));

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
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Procedure::getEntityTypeStatic(), "baz"}},
            RelPair{CallsStarRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
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
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<ModifiesRelationship*> const* modifiesRels =
            reinterpret_cast<std::vector<ModifiesRelationship*> const*>(
                queryFacade->getRelationshipsByTypes(ModifiesRelationship::getRelationshipTypeStatic(),
                                                     Procedure::getEntityTypeStatic(), Variable::getEntityTypeStatic()));

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
                    {Procedure::getEntityTypeStatic(), "baz"},
                    {Variable::getEntityTypeStatic(), "x"}},
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
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::vector<ModifiesRelationship*> const* modifiesRels =
            reinterpret_cast<std::vector<ModifiesRelationship*> const*>(
                queryFacade->getRelationshipsByTypes(ModifiesRelationship::getRelationshipTypeStatic(),
                                                     Statement::getEntityTypeStatic(), Variable::getEntityTypeStatic()));

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
                    {AssignStatement::getEntityTypeStatic(), "7"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {IfStatement::getEntityTypeStatic(), "8"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "9"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {AssignStatement::getEntityTypeStatic(), "11"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "y"}},
            RelPair{ModifiesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "z"}}};

        RequireRelationshipsMatch(modifiesRels, expectedRels);
      }

      THEN("The PKB should contain all proc-var Uses relationships") {
        auto* usesRels =
            queryFacade->getRelationshipsByTypes(UsesRelationship::getRelationshipTypeStatic(), Procedure::getEntityTypeStatic(), Variable::getEntityTypeStatic());

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "main"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "foo"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "bar"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "qux"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {Procedure::getEntityTypeStatic(), "quux"},
                    {Variable::getEntityTypeStatic(), "z"}}};

        RequireRelationshipsMatch(usesRels, expectedRels);
      }

      THEN("The PKB should contain all stmt-var Uses relationships") {
        auto* usesRels =
            queryFacade->getRelationshipsByTypes(UsesRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), Variable::getEntityTypeStatic());

        std::unordered_set<RelPair, RelPair::Hasher> expectedRels = {
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {PrintStatement::getEntityTypeStatic(), "3"},
                    {Variable::getEntityTypeStatic(), "z"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "4"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "4"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "6"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "6"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {IfStatement::getEntityTypeStatic(), "8"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {IfStatement::getEntityTypeStatic(), "8"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "10"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {PrintStatement::getEntityTypeStatic(), "12"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "w"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "x"}},
            RelPair{UsesRelationship::getRelationshipTypeStatic(),
                    {CallStatement::getEntityTypeStatic(), "13"},
                    {Variable::getEntityTypeStatic(), "z"}}};

        RequireRelationshipsMatch(usesRels, expectedRels);
      }
    }
  }
}
}  // namespace test_frontend

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
