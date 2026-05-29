#if !defined(AURORAL_HEADER_H)
#define AURORAL_HEADER_H

#include <cassert>
#include <cctype>
#include <complex>
#include <cstdarg>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
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

#endif // AURORAL_HEADER_H
