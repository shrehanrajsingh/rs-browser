#if !defined(SOLSTICE_TYPES_HPP)
#define SOLSTICE_TYPES_HPP

#include "header.hpp"

namespace solstice
{
using sol_string = std::string;
template <typename T> using sol_vec = std::vector<T>;
template <typename U, typename V> using sol_map = std::map<U, V>;
template <typename T> using sol_stack = std::stack<T>;

#if defined(INT64_MAX)
using sol_int = int64_t;
#define SOL_INT_IS_64
#else
using sol_int = int;
#endif
} // namespace solstice

#endif // SOLSTICE_TYPES_HPP
