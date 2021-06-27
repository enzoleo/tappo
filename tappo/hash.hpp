#ifndef TAPPO_HASH_HPP_
#define TAPPO_HASH_HPP_

namespace tappo {

// Nested hash namespace for some custom hash functions.
namespace hash {

// The generic implementation of hash_combine function from boost.
// A good hash is important to the performance of unordered_map.
// This function is widely-used, and kind of general.
template<typename T>
inline void hash_combine(std::size_t& seed, const T& val) {
  seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

} // namespace hash

} // namespace tappo

#endif // TAPPO_HASH_HPP_
