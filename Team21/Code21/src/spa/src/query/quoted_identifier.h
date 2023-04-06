#pragma once

#include <iostream>

namespace qps {

// Class QuotedIdentifier represents a quoted identifier in stmtRef or entRef.
class QuotedIdentifier {
 private:
  std::string id;

 public:
  explicit QuotedIdentifier(const std::string &id_);
  auto getQuotedId() const -> const std::string &;

  static auto isValidId(const std::string &iden) -> bool;

  auto operator==(QuotedIdentifier const &quote_id2) const -> bool {
    return id == quote_id2.id;
  }

  friend auto operator<<(std::ostream &osstream, QuotedIdentifier const &quote_id) -> std::ostream & {
    osstream << quote_id.id;
    return osstream;
  }
};
}  // namespace qps
