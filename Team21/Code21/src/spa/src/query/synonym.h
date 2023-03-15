#pragma once

#include <iostream>
#include <string>

namespace qps {

// Synonym class represents a PQL synonym.
class Synonym {
 private:
  std::string synonym;

 public:
  Synonym(std::string syn);
  std::string getSynonym() const;
  static bool isValidSynonym(std::string syn);

  bool operator==(const Synonym &anotherSyn) const {
    return synonym == anotherSyn.synonym;
  }

  bool operator<(const Synonym &anotherSyn) const {
    return synonym < anotherSyn.synonym;
  }

  friend std::ostream &operator<<(std::ostream &os, Synonym const &syn) {
    os << syn.synonym;
    return os;
  }
};
}

namespace std {
template<>
struct hash<qps::Synonym> {
  size_t operator()(const qps::Synonym &syn) const {
    return hash<std::string>{}(syn.getSynonym());
  }
};
}
