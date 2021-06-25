#ifndef TAPPO_ARG_HPP_
#define TAPPO_ARG_HPP_

#include "opt.hpp"

namespace tappo {

class ArgBase {
public:
  // The default constructor is explicitly deleted.
  // An argument must be declared with options (descriptions are optional).
  ArgBase() = delete;

  // Accessors/mutators.
  const auto& desc() const { return desc_; }

protected:
  // The description of this argument. Usually one should clearly speficy how
  // this argument work in its decription string. However, it is allowed to
  // be empty if not specified, so we declare it optional.
  std::optional<std::string> desc_;
};

} // namespace tappo

#endif // TAPPO_ARG_HPP_
