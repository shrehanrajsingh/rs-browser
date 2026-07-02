#if !defined(SOLSTICE_VM_HPP)
#define SOLSTICE_VM_HPP

#include "codegen.hpp"
#include "const.hpp"
#include "expr.hpp"
#include "frame.hpp"
#include "header.hpp"
#include "obj.hpp"
#include "objstore.hpp"
#include "stmt.hpp"
#include "types.hpp"

namespace solstice
{
class cg_dbg_t
{
public:
  size_t idx;
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

  cg_dbg_t (cg_dbg_t *_C = nullptr, cg_dbg_t *_P = nullptr)
      : c{ _C }, p{ _P }, idx{ 0 }
  {
  }

  inline void
  vim_add_if_not_exists (sol_string s, size_t i)
  {
    if (!vim.count (s))
      vim[s] = i;
  }

  inline void
  vim_add (sol_string s, size_t i)
  {
    vim[s] = i;
  }

  size_t
  vim_get (const sol_string &s)
  {
    if (!vim.count (s))
      throw std::invalid_argument ("key not found: " + sol_string{ s });

    return vim[s];
  }

  inline bool
  vim_exists (const sol_string &s)
  {
    return !!vim.count (s);
  }

  ~cg_dbg_t () {}
};

class vm_t
{
public:
  sol_vec<bytecode_t> c;
  size_t ip;

  sol_vec<Object *> g;    /* globals */
  sol_stack<frame_t *> f; /* frames */
  sol_stack<Object *> s;  /* stack */
  ObjectStore *o;         /* object store */

  sol_vec<Constant *> table_const; /* table of constants */

  cg_dbg_t *cgi; /* codegen info */

  vm_t () : cgi{ new cg_dbg_t () }, ip{ 0 }, o{ new ObjectStore () } {}

  vm_t (sol_vec<bytecode_t> _C)
      : c{ _C }, cgi{ new cg_dbg_t () }, ip{ 0 }, o{ new ObjectStore () }
  {
  }

  void add_to_tc_if_not_exists (Constant *);

  void execute ();

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

    if (o != nullptr)
      delete o;
    o = nullptr;
  }
};

void codegen (vm_t *, sol_vec<Statement *> &);
} // namespace solstice

#endif // SOLSTICE_VM_HPP
