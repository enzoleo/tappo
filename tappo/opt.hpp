#ifndef TAPPO_OPT_HPP_
#define TAPPO_OPT_HPP_

#include <optional>
#include <string>

namespace tappo {

class Option {
public:
  // The default constructor is explicitly deleted.
  Option() = delete;
  Option(char c) : sopt_(c) { }
  Option(const std::string& s) : lopt_(s) { }
  Option(char c, const std::string& s) : sopt_(c), lopt_(s) { }

protected:
  // A short option should only contain a single character.
  std::optional<char> sopt_;

  // A long option contains a string consisting of multiple characters.
  // Note that neither the short one nor the long one is required strictly,
  // however, either of them should be activated.
  std::optional<std::string> lopt_;
};

} // namespace tappo

#endif // TAPPO_OPT_HPP_
