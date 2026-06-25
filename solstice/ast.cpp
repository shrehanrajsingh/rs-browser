#include "ast.hpp"

namespace solstice
{
Expression *_expr_getone (char *);
Statement *_stmt_getone (char *);

bool
_peek_top (char *s, const char *d)
{
  for (size_t i = 0; d[i] != '\0'; i++)
    {
      if (s[i] != d[i])
        return false;
    }

  return true;
}

void
_eat_spaces (char *&p)
{
  while (*p != '\0' && isspace (*p))
    p++;
}

/*  not using a tokenizer since I do not have time. This is a
    simple code to AST converter using string comparisons */
sol_vec<Statement *>
make_ast (sol_string s)
{
  char *p = const_cast<char *> (s.c_str ());
  Statement *st = nullptr;
  sol_vec<Statement *> res;

  while ((st = _stmt_getone (p)) != nullptr)
    res.push_back (st);

  return res;
}

Statement *
_stmt_getone (char *p)
{
  char c = *p;
  _eat_spaces (p);

  if (c == '\0')
    return nullptr;

  if (_peek_top (p, "let ")) /* the end space is important since that makes is
                                a full token */
    {
      p += 4;
      _eat_spaces (p);

      char name[64] = { 0 };
      char *np = name;

      while (*p != '\0' && isalnum (*p))
        *np++ = *p++;

      _eat_spaces (p);

      if (*p != '=')
        {
          fprintf (stderr, "syntax error, expected '=', got '%c'\n", *p);
          exit (-1);
        }

      p++; /* eat '=' */
      _eat_spaces (p);

      char *pp = p;
      int gb = 0;
      bool in_str = false;
      char stk;

      char d = *p;
      while (d != '\0')
        {
          switch (d)
            {
            case '{':
            case '(':
            case '[':
              {
                if (!in_str)
                  gb++;
              }
              break;

            case '}':
            case ')':
            case ']':
              {
                if (!in_str)
                  gb--;
              }
              break;

            case '\'':
            case '\"':
            case '`':
              {
                if (*(p - 1) == '\\' && *(p - 2) == '\\' && d == stk)
                  {
                    if (!in_str)
                      stk = c;

                    in_str = !in_str;
                  }
              }
              break;

            case ';':
              {
                if (!gb)
                  {
                    goto e1;
                  }
              }
              break;

            default:
              break;
            }

          d = *p++;
        }

    e1:;

      char *val_bk = new char[p - pp + 1];
      strncpy (val_bk, pp, p - pp);
      val_bk[p - pp] = '\0';

      LOG ("parsed var_decl, name: %s, val: %s\n", name, val_bk);

      stmt_vardecl *vd = new stmt_vardecl (name, _expr_getone (val_bk));

      delete[] val_bk;
      return static_cast<Statement *> (vd);
    }

  return nullptr;
}

Expression *
_expr_getone (char *p)
{
  Expression *res = nullptr;

  int gb = 0;
  bool in_str = false;
  char stk;

  _eat_spaces (p);

  char c = *p;
  while (c != '\0')
    {
      switch (c)
        {
        case '{':
        case '(':
        case '[':
          {
            if (!in_str)
              gb++;
          }
          break;

        case '}':
        case ')':
        case ']':
          {
            if (!in_str)
              gb--;
          }
          break;

        case '\'':
        case '\"':
        case '`':
          {
            if (*(p - 1) == '\\' && *(p - 2) == '\\' && c == stk)
              {
                if (!in_str)
                  stk = c;

                in_str = !in_str;
              }
          }
          break;

        case ';':
          {
            if (!gb)
              {
                goto e1;
              }
          }
          break;

        default:
          break;
        }

      if (isnumber (c) && !in_str)
        {
          /* TODO */
        }

      c = *p++;
    }

e1:;
  return res;
}
} // namespace solstice
