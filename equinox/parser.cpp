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
  rdp = const_cast<char *> (raw_data.data ());

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

  if (!key.size ())
    return;

  LOG ("parsing attribute '%s'\n", key.data ());
  if (key == "/" && *rdp != '\0' && *rdp == '>')
    {
      n->nd.is_inline_elem = true;
      return;
    }

  if (std::isspace (static_cast<unsigned char> (*rdp)) || *rdp == '>')
    {
      LOG ("attribute '%s' has no value\n", key.data ());
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

              val.push_back (c);
            }

          if (!mature_end)
            {
              LOG ("Invalid Syntax. Aborting...\n");
              get_error () = EQ_HP_INVALID_SYNTAX;
              return;
            }

          LOG ("attribute '%s' has value = %s\n", key.data (), val.data ());
          n->nd.set_attr (key, val);
        }
      else
        {
          LOG ("Syntax Error. Aborting...\n");
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

  LOG ("tag name = '%s'\n", tag_name.data ());
  n->nd.name = tag_name;

  /* attribute parse loop */
  //   if (*rdp != '>')
  // {
  while (*rdp != '>')
    {
      _parser_parse_attr (n);
    }
  // }

  ++rdp; // eat '>'
  return n;
}

Node *
HTMLParser::_parser_make_text_tag ()
{
  Node *n = new Node ();
  n->nd.name = "[text]";
  std::string tval = "";

  while (*rdp != '\0' && *rdp != '<')
    tval.push_back (*rdp++); /* keep it simple */

  LOG ("parsed [text] with value = '%s'\n", tval.data ());

  return n;
}

void
HTMLParser::build_tree ()
{
  _set_ptrs ();
  _init_root ();
  _skip_whitespaces ();

  std::stack<Node *> q;
  q.push (root);

  if (*rdp++ == '<' && _parser_next (const_cast<char *> ("html")))
    {
      _parser_goto_endofbracket ();

      if (get_error ())
        {
          return;
        }

      while (!_parser_stop () && !get_error ())
        {
          _skip_whitespaces ();

          LOG ("parsing *rdp = '%c'\n", *rdp);
          if (*rdp == '<')
            {
              char nxt = *(rdp + 1);

              if (nxt == '/')
                {
                  LOG ("parsing closing tag '%s'\n",
                       q.top ()->nd.name.data ());
                  char *s = _parser_snapshot () + 2 /* eat </ */;
                  _parser_goto_endofbracket ();

                  if (get_error ())
                    return;

                  *(rdp - 1) = '\0';

                  if (q.empty () || q.top ()->nd.name != s)
                    {
                      if (q.empty ())
                        {
                          LOG ("Empty queue. Aborting...\n");
                        }
                      else
                        {
                          LOG ("Invalid closing tag mismatch '%s' and '%s'\n",
                               q.top ()->nd.name.data (), s);
                        }

                      get_error () = EQ_HP_INVALID_CLOSING_TAG;
                      return;
                    }

                  Node *b = q.top ();
                  q.pop ();
                  if (q.empty ())
                    {
                      LOG ("Empty queue. Aborting...\n");
                      get_error () = EQ_HP_NO_HTML_TAG;
                      return;
                    }

                  b->parent = q.top ();
                  q.top ()->children.push_back (b);
                  Node::add_next (q.top (), b);
                }
              else
                {
                  ++rdp; // eat '<'
                  Node *t = _parser_maketag_withattrs ();

                  if (q.empty ())
                    {
                      get_error () = EQ_HP_INVALID_CLOSING_TAG;
                      return;
                    }

                  t->parent = q.top ();
                  q.top ()->children.push_back (t);
                  if (t->nd.is_inline_elem)
                    {
                      LOG ("tag '%s' is an inline tag\n", t->nd.name.data ());

                      Node::add_next (q.top (), t);
                    }
                  else
                    {
                      LOG ("tag '%s' is not an inline tag\n",
                           t->nd.name.data ());

                      q.push (t);
                    }
                }
            }
          else
            {
              /* text */
              Node *n = _parser_make_text_tag ();

              if (get_error ())
                return;

              if (q.empty ())
                {
                  get_error () = EQ_HP_INVALID_CLOSING_TAG;
                  return;
                }

              n->parent = q.top ();
              q.top ()->children.push_back (n);
              Node::add_next (q.top (), n);
            }
        }
    }
  else
    {
      get_error () = HTMLParser::EQ_HP_INVALID_SYNTAX;
      return;
    }
}

std::vector<Node *>
HTMLParser::get_tag_fromname (std::string s)
{
  std::vector<Node *> r;

  Node *n = root;
  while (n != nullptr)
    {
      if (n->nd.name == s)
        r.push_back (n);

      n = n->next;
    }

  return r;
}

Node *
HTMLParser::get_tag_fromstr (std::string v)
{
  std::vector<std::string> v_sp;

  size_t rf = 0;
  size_t lr = 0;
  while ((rf = v.find (' ', rf + 1)) != std::string::npos)
    {
      v_sp.push_back (v.substr (lr, rf - lr));
      lr = rf + 1;
    }

  v_sp.push_back (v.substr (lr, rf - lr));

  for (auto &i : v_sp)
    {
      LOG ("processing '%s'\n", i.data ());
    }

  size_t ti = 0;
  std::vector<Node *> tagvs = get_tag_fromname (v_sp[0]);

  for (Node *&i : tagvs)
    {
      /* TODO */
    }

  return nullptr;
}
} // namespace equinox
