#include "vm.hpp"

namespace solstice
{
/* modify vm.c directly */
void _codegen_stmt_one (vm_t *&, Statement *&);
void _codegen_expr_one (vm_t *&, Statement *&);

void
codegen (vm_t *vm, const sol_vec<Statement *> &s)
{
  for (Statement *&i : s)
    {
      _codegen_stmt_one (vm, i);
    }
}

void
_codegen_stmt_one (vm_t *&vm, Statement *&s)
{
  switch (s->get_type ())
    {
    case StmtType::STMT_FUNCALL:
      {
      }
      break;

    case StmtType::STMT_FUNDECL:
      {
      }
      break;

    case StmtType::STMT_IFBLOCK:
      {
      }
      break;

    case StmtType::STMT_VARDECL:
      {
        stmt_vardecl *i = static_cast<stmt_vardecl *> (s);
        _codegen_expr_one (vm, i->val);
      }
      break;

    default:
      break;
    }
}

void
_codegen_expr_one (vm_t *&vm, Expression *&s)
{
}
} // namespace solstice
