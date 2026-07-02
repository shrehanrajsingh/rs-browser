#if !defined(SOLSTICE_OBJSTORE_HPP)
#define SOLSTICE_OBJSTORE_HPP

#include "expr.hpp"
#include "header.hpp"
#include "obj.hpp"
#include "stmt.hpp"
#include "types.hpp"

namespace solstice
{
/**
 * All object requests will be delegated to a custom
 * object store which will be used by each VM instance.
 * This can be globalized, when I think of multi-threading
 */
class ObjectStore
{
  sol_vec<Object *> o;        /* objects */
  sol_map<Object *, int> oim; /* object-index map */
  sol_stack<size_t>
      free_idxs; /* objects that are free'd have empty slots available */

public:
  ObjectStore () {}
  ~ObjectStore () {}

  Object *from_const (Constant *);
  Object *request (ObjType);
  void add (Object *);
  void check_obj (Object *);
  void inc_and_check (Object *);
  void dec_and_check (Object *);
};
} // namespace solstice

#endif // SOLSTICE_OBJSTORE_HPP
