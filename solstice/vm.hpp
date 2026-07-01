#if !defined(SOLSTICE_VM_HPP)
#define SOLSTICE_VM_HPP

#include "codegen.hpp"
#include "expr.hpp"
#include "frame.hpp"
#include "header.hpp"
#include "obj.hpp"
#include "stmt.hpp"
#include "types.hpp"

namespace solstice
{
class vm_t
{
public:
  bytecode_t c;

  sol_vec<Object *> g;    /* globals */
  sol_stack<frame_t *> f; /* frames */
  sol_stack<Object *> s;  /* stack */

  vm_t () {}
  ~vm_t () {}
};
} // namespace solstice

#endif // SOLSTICE_VM_HPP
