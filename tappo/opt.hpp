#ifndef TAPPO_OPT_HPP_
#define TAPPO_OPT_HPP_

#include <optional>
#include <string>
#include "hash.hpp"

namespace tappo {

class Option {
public:
  // The default constructor is explicitly deleted.
  Option() = delete;
  Option(char c) : sopt_(c) { }
  Option(const std::string& s) : lopt_(s) { }
  Option(char c, const std::string& s) : sopt_(c), lopt_(s) { }

  // Accessors/mutators.
  const auto& sopt() const noexcept { return sopt_; }
  const auto& lopt() const noexcept { return lopt_; }

protected:
  // A short option should only contain a single character.
  std::optional<char> sopt_;

  // A long option contains a string consisting of multiple characters.
  // Note that neither the short one nor the long one is required strictly,
  // however, either of them should be activated.
  std::optional<std::string> lopt_;
};

} // namespace tappo

///////////////////////////////////////////////////////////////////////////
// Custom specialization of std::hash injected in namespace std.
namespace std {

template<>
struct hash<tappo::Option> {
  std::size_t operator()(tappo::Option const& opt) const noexcept {
    std::size_t h1 = std::hash<std::optional<char> >{ }(opt.sopt());
    std::size_t h2 = std::hash<std::optional<std::string> >{ }(opt.lopt());
    // The combination number h1 ^ (h2 << 1) is also available.
    // Refer to examples in https://en.cppreference.com/w/cpp/utility/hash
    return tappo::hash::hash_combine(h1, h2);
  }
};

} // namespace std

#endif // TAPPO_OPT_HPP_
