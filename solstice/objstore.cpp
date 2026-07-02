#include "objstore.hpp"

namespace solstice
{
Object *
ObjectStore::from_const (Constant *c)
{
  obj_const *co = new obj_const (c);
  oim[co] = o.size ();
  o.push_back (co);

  return static_cast<Object *> (co);
}

Object *
ObjectStore::request (ObjType t)
{
  Object *res = nullptr;

  switch (t)
    {
    case ObjType::OBJ_CONST:
      {
        obj_const *co = new obj_const (nullptr);
        res = static_cast<Object *> (co);
      }
      break;

    case ObjType::OBJ_FUNC:
      {
        obj_func *fo = new obj_func ("", 0);
        res = static_cast<Object *> (fo);
      }
      break;

    default:
      break;
    }

  if (res == nullptr)
    throw std::runtime_error ("[ObjectStore] Invalid ObjType.\n");

  oim[res] = o.size ();
  o.push_back (res);
  return res;
}

void
ObjectStore::add (Object *s)
{
  oim[s] = o.size ();
  o.push_back (s);
}

void
ObjectStore::check_obj (Object *i)
{
  if (i->get_count () < 1)
    {
      if (!oim.count (i))
        throw std::invalid_argument ("[ObjectStore] Object not found.\n");

      free_idxs.push (oim[i]);
      oim.erase (i);
    }
}

void
ObjectStore::inc_and_check (Object *i)
{
  i->inc_count ();
  /* no need to check here??? */
}

void
ObjectStore::dec_and_check (Object *i)
{
  i->dec_count ();
  check_obj (i);
}
} // namespace solstice
