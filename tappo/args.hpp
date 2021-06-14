#ifndef TAPPO_ARGS_HPP_
#define TAPPO_ARGS_HPP_

#include <string>

namespace tappo {

class ArgBase {
public:
  // The default constructor.
  ArgBase() = default;

protected:
  // The description of this argument.
  std::string desc_;
};

} // namespace tappo

#endif // TAPPO_ARGS_HPP_
