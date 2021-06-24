#ifndef TAPPO_ARGS_HPP_
#define TAPPO_ARGS_HPP_

#include <optional>
#include <string>

namespace tappo {

class ArgBase {
public:
  // The default constructor.
  ArgBase() = default;

  // Accessors/mutators.
  const auto& desc() const { return desc_; }

protected:
  // The description of this argument. Usually one should clearly speficy how
  // this argument work in its decription string. However, it is allowed to
  // be empty if not specified, so we declare it optional.
  std::optional<std::string> desc_;
};

} // namespace tappo

#endif // TAPPO_ARGS_HPP_
