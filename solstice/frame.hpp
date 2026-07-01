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

  inline Object *&
  get_local (size_t i)
  {
    if (i > l.size ())
      throw std::out_of_range ("Invalid local access");

    return l[i];
  }
};
} // namespace solstice

#endif // SOLSTICE_FRAME_HPP
