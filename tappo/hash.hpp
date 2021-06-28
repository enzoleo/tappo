#ifndef TAPPO_HASH_HPP_
#define TAPPO_HASH_HPP_

namespace tappo {

// Nested hash namespace for some custom hash functions.
namespace hash {

namespace detail {

// The generic implementation of hash_combine function from boost with some
// customizations. A good hash function is important to the performance of
// unordered_map. This function is widely-used, and kind of general.
template<typename T>
inline void _hash_combine_impl(std::size_t& seed, T&& val) {
  seed ^= std::hash<std::decay_t<T> >()(std::forward<T>(val))
        + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T, typename... Ts>
inline void _hash_combine_impl(std::size_t& seed, T&& val, Ts&&... vals) {
  _hash_combine_impl(seed, std::forward<T>(val));
  _hash_combine_impl(seed, std::forward<Ts>(vals)...);
}

} // namespace detail

// The generic hash_combine function that combines multiple hash values
// into a single one from zero seed as the initial value.
template<typename... Ts>
inline std::size_t hash_combine(Ts&&... vals) {
  // Start from the initial seed zero.
  std::size_t seed = 0;
  detail::_hash_combine_impl(seed, std::forward<Ts>(vals)...);
  return seed;
}

} // namespace hash

} // namespace tappo

#endif // TAPPO_HASH_HPP_
