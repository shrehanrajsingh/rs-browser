#include "const.hpp"

namespace solstice
{
void
print_constant (Constant *c)
{
  if (c == nullptr)
    {
      std::cout << "<null constant>\n";
      return;
    }

  switch (c->get_type ())
    {
    case ConstType::CONST_INT:
      {
        auto *v = static_cast<const_int *> (c);
        std::cout << "CONST_INT: " << v->v << '\n';
        break;
      }

    case ConstType::CONST_FLOAT:
      {
        auto *v = static_cast<const_float *> (c);
        std::cout << "CONST_FLOAT: " << v->v << '\n';
        break;
      }

    case ConstType::CONST_STRING:
      {
        auto *v = static_cast<const_string *> (c);
        std::cout << "CONST_STRING: \"" << v->v << "\"\n";
        break;
      }

    case ConstType::CONST_BOOL:
      {
        auto *v = static_cast<const_bool *> (c);
        std::cout << "CONST_BOOL: " << (v->v ? "true" : "false") << '\n';
        break;
      }

    case ConstType::CONST_UNDEFINED:
      {
        std::cout << "CONST_UNDEFINED\n";
        break;
      }
    }
}

bool
const_eqeq (Constant *&a, Constant *&b)
{
  if (a->get_type () != b->get_type ())
    return false;

  switch (a->get_type ())
    {
    case ConstType::CONST_BOOL:
      {
        return dynamic_cast<const_bool *> (a)->v
               == dynamic_cast<const_bool *> (b)->v;
      }
      break;

    case ConstType::CONST_FLOAT:
      {
        return dynamic_cast<const_float *> (a)->v
               == dynamic_cast<const_float *> (b)->v;
      }
      break;

    case ConstType::CONST_INT:
      {
        return dynamic_cast<const_int *> (a)->v
               == dynamic_cast<const_int *> (b)->v;
      }
      break;

    case ConstType::CONST_STRING:
      {
        return dynamic_cast<const_string *> (a)->v
               == dynamic_cast<const_string *> (b)->v;
      }
      break;

    case ConstType::CONST_UNDEFINED:
      return true; /* both types are same */
      break;

    default:
      break;
    }

  return false;
}
} // namespace solstice
