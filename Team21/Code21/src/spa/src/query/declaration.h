#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "query/design_entity.h"
#include "query/synonym.h"
#include <optional>

namespace qps {

class Declaration {

private:
  DesignEntity design_entity;
  Synonym synonym;

public:
  auto getDesignEntity() -> DesignEntity;
  auto getSynonym() -> Synonym;
  Declaration(DesignEntity design_entity_, Synonym synonym_);

  auto operator==(const Declaration &declaration2) const -> bool {
    return design_entity == declaration2.design_entity &&
           synonym == declaration2.synonym;
  }

  friend auto operator<<(std::ostream &ostream, Declaration const &declaration)
      -> std::ostream & {
    ostream << getStringFromDE(declaration.design_entity) << " "
            << declaration.synonym;
    return ostream;
  }

  static auto findDeclarationWithSynonym(std::vector<Declaration> &decl_list,
                                         Synonym &synonym)
      -> std::optional<Declaration> {
    std::string synonym_ = synonym.getSynonym();
    return findDeclarationWithString(decl_list, synonym_);
  }

  static auto findDeclarationWithString(std::vector<Declaration> &decl_list,
                                        std::string &syn_string)
      -> std::optional<Declaration> {
    for (auto &idx : decl_list) {
      if (idx.getSynonym().getSynonym() == syn_string) {
        return std::make_optional<Declaration>(idx);
      }
    }
    return std::nullopt;
  }
};
} // namespace qps
