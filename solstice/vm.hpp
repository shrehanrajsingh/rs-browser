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
class cg_dbg_t
{
public:
  sol_map<sol_string, size_t> vim; /* var-index map */

  /**
   * as we go down in function definitions,
   * we need new debug frames to resolve
   * variables properly.
   * For this reason we treat the debug
   * struct as a linear data structure
   * growing down (one direction, 2-way)
   * We also need a way to come up once
   * the underlying function definition is over.
   * For this reason, we also have a parent.
   * So in short, a doubly-linked list
   */
  cg_dbg_t *c; /* child */
  cg_dbg_t *p; /* parent */

  cg_dbg_t (cg_dbg_t *_C = nullptr, cg_dbg_t *_P = nullptr) : c{ _C }, p{ _P }
  {
  }

  ~cg_dbg_t () {}
};

class vm_t
{
public:
  sol_vec<bytecode_t> c;

  sol_vec<Object *> g;    /* globals */
  sol_stack<frame_t *> f; /* frames */
  sol_stack<Object *> s;  /* stack */

  cg_dbg_t *cgi; /* codegen info */

  vm_t () : cgi{ new cg_dbg_t () } {}
  vm (sol_vec<bytecode_t> _C) : c{ _C } {}

  ~vm_t ()
  {
    /**
     * @note we are only deleting the top of
     * the structure.
     * we need to take care of the underlying children as well
     */

    if (cgi != nullptr)
      delete cgi;
    cgi = nullptr;
  }
};

void codegen (vm_t *, const sol_vec<Statement *> &);
} // namespace solstice

#endif // SOLSTICE_VM_HPP
