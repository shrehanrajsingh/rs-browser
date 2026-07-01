#if !defined(SOLSTICE_FRAME_HPP)
#define SOLSTICE_FRAME_HPP

#include "expr.hpp"
#include "header.hpp"
#include "obj.hpp"
#include "stmt.hpp"
#include "types.hpp"

namespace solstice
{
class frame_t
{
public:
  sol_vec<Object *> l; /* locals */

  frame_t () {}
  ~frame_t () {}
};
} // namespace solstice

#endif // SOLSTICE_FRAME_HPP
