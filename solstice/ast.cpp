#include "ast.hpp"

namespace solstice
{
Expression *_expr_getone (char *&);
Statement *_stmt_getone (char *&);

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

char *
_get_num (char *p, bool &is_f)
{
  char *s = p;

l1:;
  while (*s != '\0' && isnumber (*s))
    s++;

  if (*s == '.' && !is_f)
    {
      is_f = true;
      s++;
      goto l1;
    }

  return s; /* points to token after the last number, 12345<TOK> => points at
               tok */
}

bool
_check_is_reserved (char *a, char *b)
{
  static const char *reserved[] = { "if", "else", "while", "return", NULL };

  char *ap = a;

  for (size_t i = 0; reserved[i] != NULL; i++)
    {
      const char *p = reserved[i];

      while (a != b)
        {
          if (*a == *p)
            {
              a++;
              p++;
            }
          else
            break;
        }

      if (a == b && *p == '\0')
        return true;

      a = ap;
    }

  return false;
}

char *
_get_identifier (char *p, bool &is_reserved)
{
  char *s = p;

l1:;
  while (*s != '\0' && (isalnum (*s) || *s == '_'))
    s++;

  return s;
}

/* points at end quote */
char *
_get_string (char *p)
{
  char *og = p;
  char t = *p++; /* "/'/` */
  bool in_str = false;

  char d = *p;
  while (d != '\0')
    {
      if (d == t)
        {
          bool yes_end = true;
          int blks = 0;

          const char *q = p - 1;
          while (q >= og && *q == '\\')
            {
              blks++;
              q--;
            }

          yes_end = (blks % 2 == 0);

          if (yes_end)
            goto end;
        }

      d = *p++;
    }

end:;
  return p; /* points after the end quote */
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
_stmt_getone (char *&p)
{
  _eat_spaces (p);

  char *og = p;
  char c = *p;

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
      char stk = 0;

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
                bool yes_end = false;

                if (in_str)
                  {
                    int blks = 0;

                    const char *q = p - 1;
                    while (q >= og && *q == '\\')
                      {
                        blks++;
                        q--;
                      }

                    yes_end = (blks % 2 == 0);

                    if (d != stk)
                      yes_end = false;
                  }
                else
                  {
                    stk = d;
                    in_str = true;
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
      char *vbk_p = val_bk;

      LOG ("parsed var_decl, name: %s, val: %s\n", name, val_bk);

      stmt_vardecl *vd = new stmt_vardecl (name, _expr_getone (val_bk));

      delete[] vbk_p;
      return static_cast<Statement *> (vd);
    }

  return nullptr;
}

Expression *
_expr_getone (char *&p)
{
  char *og = p;
  Expression *res = nullptr;

  int gb = 0;
  bool in_str = false;
  char stk = 0;

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
            bool yes_end = false;

            if (in_str)
              {
                int blks = 0;

                const char *q = p - 1;
                while (q >= og && *q == '\\')
                  {
                    blks++;
                    q--;
                  }

                yes_end = (blks % 2 == 0);

                if (c != stk)
                  yes_end = false;
              }

            LOG ("%d\n", yes_end);

            if (yes_end)
              {
                if (!in_str)
                  stk = c;

                in_str = !in_str;
              }
            else
              in_str = true;
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

      if (!in_str)
        {
          if (isnumber (c))
            {
              bool is_f = false;
              char *nt = _get_num (p, is_f);

              char *pp = new char[nt - p + 1];
              strncpy (pp, p, nt - p);
              pp[nt - p] = '\0';
              char *endptr = nullptr;

              LOG ("parsed number %s\n", pp);

              try
                {
                  if (is_f)
                    {
                      res = static_cast<Expression *> (new expr_const (
                          static_cast<Constant *> (new const_float (
                              static_cast<float> (std::stod (pp))))));
                    }
                  else
                    {
                      res = static_cast<Expression *> (new expr_const (
                          static_cast<Constant *> (new const_int (
                              static_cast<int> (std::stod (pp))))));
                    }
                }
              catch (const std::invalid_argument &e)
                {
                  fprintf (stderr, "invalid number %s\n", pp);
                  delete[] pp;

                  exit (-1);
                }
              catch (const std::out_of_range &e)
                {
#ifdef SOL_INT_IS_64
                  fprintf (stderr, "number out of range of 64 bit integer\n");
#else
                  fprintf (stderr, "number out of range\n");
#endif

                  exit (-1);
                }

              delete[] pp;

              p = nt;
            }
          else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                   || c == '_')
            {
              LOG ("alpha: %c %s\n", c, p);
              bool is_r = false;
              char *nt = _get_identifier (p, is_r);

              char *pp = new char[nt - p + 1];
              strncpy (pp, p, nt - p);
              pp[nt - p] = '\0';

              bool isr = _check_is_reserved (p, nt);
              LOG ("parsed identifier %s (is reserved: %d)\n", pp,
                   _check_is_reserved (p, nt));

              if (!strcmp (pp, "true") || !strcmp (pp, "false"))
                {
                  LOG ("got boolean: %s\n", pp);
                  res = static_cast<Expression *> (new expr_const (
                      static_cast<Constant *> (new const_bool (*pp == 't'))));
                }
              else if (isr)
                {
                  here;
                  LOG ("TODO: NOT IMPLEMENTED YET\n");
                  exit (1);
                  //...
                }
              else
                res = static_cast<Expression *> (new expr_var (pp));

              delete[] pp;
              p = nt;
            }
        }
      else
        {
          char *s = _get_string (p);

          char *pp = new char[s - p - 1];
          strncpy (pp, p + 1, s - p - 1);
          pp[s - p - 2] = '\0';

          LOG ("parsed string %s\n", pp);

          res = static_cast<Expression *> (new expr_const (
              static_cast<Constant *> (new const_string (pp))));

          delete[] pp;
          p = s;
          in_str = false;
          stk = 0;

          LOG ("p at '%s'\n", p);
        }

      c = *p++;
    }

e1:;
  return res;
}
} // namespace solstice
