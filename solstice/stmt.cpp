#include "stmt.hpp"
#include "expr.hpp"

namespace solstice
{
void
print_statement (Statement *s)
{
  if (s == nullptr)
    {
      std::cout << "<null statement>\n";
      return;
    }

  switch (s->get_type ())
    {
    case StmtType::STMT_VARDECL:
      {
        auto *v = static_cast<stmt_vardecl *> (s);

        std::cout << "STMT_VARDECL\n";
        std::cout << "  name: " << v->name << '\n';
        std::cout << "  value: ";
        print_expr (v->val);
        break;
      }

    case StmtType::STMT_FUNCALL:
      {
        auto *v = static_cast<stmt_funcall *> (s);

        std::cout << "STMT_FUNCALL\n";
        std::cout << "  function:\n";
        print_expr (v->name);

        std::cout << "  args (" << v->args.size () << "):\n";
        for (size_t i = 0; i < v->args.size (); i++)
          {
            std::cout << "    [" << i << "] ";
            print_expr (v->args[i]);
          }
        break;
      }

    case StmtType::STMT_IFBLOCK:
      {
        auto *v = static_cast<stmt_ifblock *> (s);

        std::cout << "STMT_IFBLOCK\n";
        std::cout << "  condition:\n";
        print_expr (v->cond);

        std::cout << "  body (" << v->body.size () << "):\n";
        for (size_t i = 0; i < v->body.size (); i++)
          {
            std::cout << "    [" << i << "]\n";
            print_statement (v->body[i]);
          }
        break;
      }

    case StmtType::STMT_FUNDECL:
      {
        auto *v = static_cast<stmt_fundecl *> (s);

        std::cout << "STMT_FUNDECL\n";
        std::cout << "  name: " << v->name << '\n';

        std::cout << "  parameters (" << v->args.size () << "):";
        for (size_t i = 0; i < v->args.size (); i++)
          std::cout << ' ' << v->args[i];
        std::cout << '\n';

        std::cout << "  body (" << v->body.size () << "):\n";
        for (size_t i = 0; i < v->body.size (); i++)
          {
            std::cout << "    [" << i << "]\n";
            print_statement (v->body[i]);
          }
        break;
      }
    }
}
} // namespace solstice
