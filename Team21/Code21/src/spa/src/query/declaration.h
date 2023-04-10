#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "query/design_entity.h"
#include "query/synonym.h"
#include <optional>

namespace qps {

// Declaration represents the declaration of a synonym with design entity
class Declaration {
 private:
  DesignEntity design_entity;
  Synonym synonym;

 public:
  [[nodiscard]] auto getDesignEntity() const -> DesignEntity;
  [[nodiscard]] auto getSynonym() const -> const Synonym &;
  Declaration(DesignEntity design_entity_, Synonym synonym_);

  auto operator==(const Declaration &declaration2) const -> bool {
    return design_entity == declaration2.design_entity &&
        synonym == declaration2.synonym;
  }

  friend auto operator<<(std::ostream &ostream, const Declaration &declaration)
  -> std::ostream & {
    ostream << getStringFromDE(declaration.design_entity) << " "
            << declaration.synonym;
    return ostream;
  }

  // Find Declaration of a synonym from the list of declarations
  static auto findDeclarationWithSynonym(const std::vector<Declaration> &decl_list,
                                         const Synonym &synonym)
  -> std::optional<Declaration> {
    return findDeclarationWithString(decl_list, synonym.getSynonym());
  }

  // Find Declaration of a synonym from the list of declarations (String form)
  static auto findDeclarationWithString(const std::vector<Declaration> &decl_list,
                                        const std::string &syn_string)
  -> std::optional<Declaration> {
    for (const auto &idx : decl_list) {
      if (idx.getSynonym().getSynonym() == syn_string) {
        return std::make_optional<Declaration>(idx);
      }
    }
    return std::nullopt;
  }
};

} // namespace qps
