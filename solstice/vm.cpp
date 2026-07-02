#include "vm.hpp"

namespace solstice
{
/* modify vm.c directly */
void _codegen_stmt_one (vm_t *&, Statement *&);
void _codegen_expr_one (vm_t *&, Expression *&);

#define ADD_BC(T, A, B, C) vm->c.push_back (bytecode_t ((T), (A), (B), (C)))

void
codegen (vm_t *vm, sol_vec<Statement *> &s)
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
        stmt_fundecl *i = static_cast<stmt_fundecl *> (s);

        sol_string &name = i->name;
        sol_vec<sol_string> &args = i->args;
        sol_vec<Statement *> &body = i->body;

        cg_dbg_t *pcgi = vm->cgi;

        vm->cgi->c = new cg_dbg_t ();
        vm->cgi->c->p = vm->cgi;
        vm->cgi = vm->cgi->c;

        ADD_BC (ByteCodeType::OP_JUMP, 0, 0, nullptr);

        size_t pip = vm->c.size ();
        bytecode_t &jc = vm->c.back ();

        for (sol_string &j : args)
          {
            vm->cgi->vim_add (j, vm->cgi->idx);
            ADD_BC (ByteCodeType::OP_STORE_LOCAL, vm->cgi->idx,
                    0 /* no levels up */, nullptr);

            vm->cgi->idx++;
          }

        for (Statement *&j : body)
          {
            _codegen_stmt_one (vm, j);
          }

        ADD_BC (ByteCodeType::OP_RETURN, 0, 0, nullptr);
        jc.get_a () = vm->c.size ();

        ADD_BC (ByteCodeType::OP_STOREFUNC_GLOBAL, vm->cgi->idx, pip, nullptr);

        delete vm->cgi;
        vm->cgi = pcgi;
        vm->cgi->c = nullptr;
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

        vm->cgi->vim_add (i->name, vm->cgi->idx);

        ADD_BC (ByteCodeType::OP_STORE_GLOBAL, vm->cgi->idx, 0, nullptr);
        vm->cgi->idx++;
      }
      break;

    default:
      break;
    }
}

void
_codegen_expr_one (vm_t *&vm, Expression *&s)
{
  switch (s->get_type ())
    {
    case ExprType::EXPR_CONST:
      {
        expr_const *i = static_cast<expr_const *> (s);
        vm->add_to_tc_if_not_exists (i->v);

        ADD_BC (ByteCodeType::OP_LOAD_CONST, vm->table_const.size () - 1, 0,
                nullptr);
      }
      break;

    case ExprType::EXPR_VAR:
      {
        expr_var *i = static_cast<expr_var *> (s);

        if (!vm->cgi->vim_exists (i->name))
          throw std::invalid_argument ("name \'" + i->name
                                       + "\' does not exist.\n");

        ADD_BC (ByteCodeType::OP_LOAD_NAME, vm->cgi->vim_get (i->name), 0,
                nullptr);
      }
      break;

    default:
      break;
    }
}

void
vm_t::add_to_tc_if_not_exists (Constant *c)
{
  bool found = false;

  for (Constant *&i : table_const)
    {
      if (const_eqeq (i, c))
        {
          found = true;
          break;
        }
    }

  if (!found)
    {
      table_const.push_back (c);
    }
}

void
vm_t::execute ()
{
  if (f.empty ())
    {
      throw std::runtime_error ("VM called with no frame.\n");
      return;
    }

  frame_t *&fr = f.top ();
  f.pop ();

  while (ip < c.size ())
    {
      bytecode_t &t = c[ip++];

      switch (t.get_type ())
        {
        case ByteCodeType::OP_LOAD_CONST:
          {
            Constant *c = table_const[t.get_a ()];
            Object *co = o->from_const (c);

            co->inc_count ();
            s.push (co);
          }
          break;

        case ByteCodeType::OP_LOAD_NAME:
          {
            int idx = t.get_a ();

            if (t.get_b ())
              {
                /* frame */
                /* TODOOOOOOOOOOOOO */

                /**
                 * this is tricky because if I need more
                 * information for recursive frame.
                 * What if the variable is in a frame a
                 * few levels up?
                 */
              }
            else
              {
                /* global */
                if (idx > g.size () || idx < 0)
                  throw std::invalid_argument ("name does not exist.\n");

                Object *nn = g[idx];

                if (nn == nullptr)
                  throw std::invalid_argument ("corrupted object.\n");

                nn->inc_count ();
                s.push (nn);
              }
          }
          break;

        case ByteCodeType::OP_STORE_LOCAL:
          {
            int idx = t.get_a ();

            if (s.empty ())
              throw std::runtime_error (
                  "nothing to store into local variable.\n");

            Object *oo = s.top ();
            s.pop ();
            /* already IR'ed */

            while (idx >= fr->l.size ())
              fr->l.push_back (nullptr);

            fr->l[idx] = oo;
          }
          break;

        case ByteCodeType::OP_STORE_GLOBAL:
          {
            int idx = t.get_a ();

            if (s.empty ())
              throw std::runtime_error ("nothing to store into variable.\n");

            Object *oo = s.top ();
            s.pop ();
            /* already IR'ed */

            while (idx >= g.size ())
              g.push_back (nullptr);

            g[idx] = oo;
          }
          break;

        default:
          break;
        }
    }
}
} // namespace solstice
