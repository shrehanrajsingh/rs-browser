#if !defined(SOLSTICE_HEADER)
#define SOLSTICE_HEADER

#include <cassert>
#include <cctype>
#include <complex>
#include <cstdarg>
#include <cstdint>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifndef here
#define here std::printf ("%s (%s): %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

#ifndef LOG
#define LOG(...)                                                              \
  {                                                                           \
    std::printf ("[LOG] (%s:%d) ", __FUNCTION__, __LINE__);                   \
    std::printf (__VA_ARGS__);                                                \
  }
#endif

#endif // SOLSTICE_HEADER
