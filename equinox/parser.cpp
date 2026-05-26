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
HTMLParser::_parser_parse_attr (Node *n)
{
  _skip_whitespaces ();

  bool saw_eq = false;
  std::string key = "";
  std::string val = "";

  while (*rdp != '\0' && *rdp != '=' && *rdp != '>'
         && !std::isspace (static_cast<unsigned char> (*rdp)))
    key.push_back (*rdp++);

  if (std::isspace (static_cast<unsigned char> (*rdp)) || *rdp == '>')
    {
      n->nd.set_attr (key, val);
      return;
    }
  else
    {
      if (*rdp == '=')
        {
          bool in_str = false;
          bool mature_end = false;
          char str_tok = 0;
          ++rdp;

          while (*rdp != '\0')
            {
              char c = *rdp++;
              val.push_back (c);

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

              if ((c == '>' || std::isspace (static_cast<unsigned char> (c)))
                  && !in_str)
                {
                  mature_end = true;
                  --rdp; /* point to '>' or ' ' */
                  break;
                }
            }

          if (!mature_end)
            {
              get_error () = EQ_HP_INVALID_SYNTAX;
              return;
            }

          n->nd.set_attr (key, val);
        }
      else
        {
          get_error () = EQ_HP_INVALID_SYNTAX;
          return;
        }
    }
}

Node *
HTMLParser::_parser_maketag_withattrs ()
{
  std::string tag_name = "";
  tag_name.reserve (16);

  Node *n = new Node ();

  while (*rdp != '\0' && !std::isspace (static_cast<unsigned char> (*rdp))
         && *rdp != '>')
    tag_name.push_back (*rdp++);

  /* attribute parse loop */
  if (*rdp == '>')
    {
      n->nd.name = tag_name;
    }
  else
    {
      while (*rdp != '>')
        {
          _parser_parse_attr (n);
        }

      ++rdp; // eat '>'
    }

  return n;
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

                  Node *b = q.pop ();
                  if (q.empty ())
                    {
                      get_error () = EQ_HP_NO_HTML_TAG;
                      return;
                    }

                  Node::add_next (q.back (), b);
                }
              else
                {
                  ++rdp;
                  Node *t = _parser_maketag_withattrs ();
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
