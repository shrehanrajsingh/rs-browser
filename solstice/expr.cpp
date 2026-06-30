#include "expr.hpp"

namespace solstice
{
void
print_expr (Expression *e)
{
  if (e == nullptr)
    {
      std::cout << "<null expression>\n";
      return;
    }

  switch (e->get_type ())
    {
    case ExprType::EXPR_CONST:
      {
        auto *v = static_cast<expr_const *> (e);
        std::cout << "EXPR_CONST\n";
        std::cout << "  value: ";
        print_constant (v->v);
        break;
      }

    case ExprType::EXPR_VAR:
      {
        auto *v = static_cast<expr_var *> (e);
        std::cout << "EXPR_VAR\n";
        std::cout << "  name: " << v->name << '\n';
        break;
      }
    }
}
} // namespace solstice
