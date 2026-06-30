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
} // namespace solstice
