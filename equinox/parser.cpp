#include "parser.hpp"

namespace equinox
{

void
HTMLParser::_skip_whitespaces ()
{
  while (*rdp != '\0' && std::isspace (static_cast<unsigned char> (*rdp)))
    ++rdp;
}

inline bool &
HTMLParser::_parser_stop ()
{
  return end;
}

void
HTMLParser::_set_ptrs ()
{
  rdp = raw_data.data ();

  if (*rdp == '\0')
    _parser_stop () = true;
}

inline void
HTMLParser::_init_root ()
{
  root = new Node ();
  root->nd.name = "html"; /* tagname */
}

inline bool
HTMLParser::has_error ()
{
  return err != 0;
}

inline int &
HTMLParser::get_error ()
{
  return err;
}

bool
HTMLParser::_parser_next (char *v)
{
  char *s = _parser_snapshot ();

  while (*v != '\0' && *rdp != '\0' && *v == *rdp)
    {
      v++;
      rdp++;
    }

  if (*v != '\0')
    {
      rdp = s;
      return false;
    }

  return true;
}

char *
HTMLParser::_parser_snapshot ()
{
  return rdp;
}

void
HTMLParser::_parser_goto_endofbracket ()
{
  bool in_str = false; /* to make sure the '>' is not inside attributes */
  char str_tok = 0;
  bool mature_end = false;

  while (*rdp != '\0')
    {
      char c = *rdp++;

      if (c == '\"' || c == '\'')
        {
          if (!in_str)
            {
              in_str = true;
              str_tok = c;
            }
          else
            {
              if (*(rdp - 2) == '\\')
                {
                  if (*(rdp - 3) == '\\')
                    {
                      in_str = false;
                      str_tok = 0;
                    }
                  else
                    {
                      /* backslash char */
                    }
                }
              else
                {
                  in_str = false;
                  str_tok = 0;
                }
            }
        }

      if (c == '>' && !in_str)
        {
          mature_end = true;
          /* rdp now points to the character after '>' */
          break;
        }
    }

  if (*rdp == '\0' && !mature_end)
    {
      get_error () = EQ_HP_NO_CLOSING_BRACKET_FOUND;
    }
}

void
HTMLParser::build_tree ()
{
  _set_ptrs ();
  _init_root ();
  _skip_whitespaces ();

  std::queue<Node *> q;
  q.push (root);

  if (*rdp++ == '<' && _parser_next ("html"))
    {
      _parser_goto_endofbracket ();

      if (get_error ())
        return;

      while (!_parser_stop () && !get_error ())
        {
          _skip_whitespaces ();

          if (*rdp == '<')
            {
              char nxt = *(rdp + 1);

              if (nxt == '/')
                {
                  char *s = _parser_snapshot ();
                  _parser_goto_endofbracket ();

                  if (get_error ())
                    return;

                  *(rdp - 1) = '\0';

                  if (q.empty () || q.back ()->nd.name != s)
                    {
                      get_error () = EQ_HP_INVALID_CLOSING_TAG;
                      return;
                    }

                  /* TODO */
                }
              else
                {
                  /* TODO */
                }
            }
          else
            {
              /* text */
              Node *n = _parser_make_text_tag ();

              if (get_error ())
                return;

              /* TODO */
            }
        }
    }
  else
    {
      get_error () = HTMLParser::EQ_HP_INVALID_SYNTAX;
      return;
    }
}
} // namespace equinox
