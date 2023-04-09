#pragma once

#include <iostream>
#include <string>

namespace qps {

// Synonym class represents a PQL synonym.
class Synonym {
 private:
  std::string synonym;

 public:
  explicit Synonym(const std::string &syn);
  [[nodiscard]] auto getSynonym() const -> const std::string &;
  static auto isValidSynonym(const std::string &syn) -> bool;

  auto operator==(const Synonym &anotherSyn) const -> bool {
    return synonym == anotherSyn.synonym;
  }

  auto operator<(const Synonym &anotherSyn) const -> bool {
    return synonym < anotherSyn.synonym;
  }

  friend auto operator<<(std::ostream &ostream, Synonym const &syn) -> std::ostream & {
    ostream << syn.synonym;
    return ostream;
  }
};
}  // namespace qps

namespace std {
template<>
struct hash<qps::Synonym> {
  auto operator()(const qps::Synonym &syn) const -> size_t {
    return hash<std::string>{}(syn.getSynonym());
  }
};
}
