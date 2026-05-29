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

  if (key == "/" && *rdp != '\0' && *rdp == '>')
    {
      n->nd.is_inline_elem = true;
      return;
    }
  LOG ("parsing attribute '%s'\n", key.data ());

  if (std::isspace (static_cast<unsigned char> (*rdp)) || *rdp == '>')
    {
      LOG ("attribute '%s' has no value\n", key.data ());

      if (val[0] == '\'' || val[0] == '\"')
        val = val.substr (1, val.size () - 2);

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

          if (val[0] == '\'' || val[0] == '\"')
            val = val.substr (1, val.size () - 2);
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

  bool last_chr_isssp = false;

  while (*rdp != '\0' && *rdp != '<')
    {
      if (std::isspace (static_cast<unsigned char> (*rdp)))
        {
          if (!last_chr_isssp)
            {
              tval.push_back (' ');
              last_chr_isssp = true;
            }

          rdp++;
          continue;
        }
      else
        last_chr_isssp = false;

      tval.push_back (*rdp++); /* keep it simple */
    }

  LOG ("parsed [text] with value = '%s'\n", tval.data ());
  n->nd.set_attr ("_", tval);

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
                  if (q.empty () && b->nd.name != "html")
                    {
                      LOG ("Empty queue. Aborting...\n");
                      get_error () = EQ_HP_NO_HTML_TAG;
                      return;
                    }
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
                  Node::add_next (q.top (), t);

                  if (t->nd.is_inline_elem)
                    {
                      LOG ("tag '%s' is an inline tag\n", t->nd.name.data ());
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

static std::vector<Node *> __eq_rt_dfs_1 (std::vector<std::string> &, size_t,
                                          std::vector<Node *> &);

std::vector<Node *>
HTMLParser::get_tag_fromstr (std::string v)
{
  std::vector<std::string> v_sp;
  std::vector<Node *> r;

  size_t rf = 0;
  size_t lr = 0;
  while ((rf = v.find (' ', rf + 1)) != std::string::npos)
    {
      v_sp.push_back (v.substr (lr, rf - lr));
      lr = rf + 1;
    }

  if (rf == std::string::npos && lr == 0)
    rf = v.size ();

  v_sp.push_back (v.substr (lr, rf - lr));

  for (auto &i : v_sp)
    {
      LOG ("processing '%s'\n", i.data ());
    }

  size_t ti = 0;
  std::vector<Node *> tagvs = get_tag_fromname (v_sp[0]);

  for (Node *&i : tagvs)
    {
      if (i->nd.name == v_sp[ti])
        {
          LOG ("got match '%s'\n", v_sp[ti].data ());

          if (ti + 1 >= v_sp.size ())
            {
              r.push_back (i);
              continue;
            }

          std::vector<Node *> p = __eq_rt_dfs_1 (v_sp, ti + 1, i->children);

          if (p.size ())
            {
              LOG ("dfs got %lu results from root at '%s'\n", p.size (),
                   v_sp[ti].data ());
              r.insert (r.end (), p.begin (), p.end ());
            }
          else
            {
              LOG ("dfs got 0 (p is empty) results from root at '%s'\n",
                   v_sp[ti].data ());
            }
        }
    }

  return r;
}

static std::vector<Node *>
__eq_rt_dfs_1 (std::vector<std::string> &vsp, size_t ti,
               std::vector<Node *> &children)
{
  std::vector<Node *> r;
  if (ti >= vsp.size ())
    return r;

  if (ti + 1 == vsp.size ()) /* last tag */
    {
      LOG ("last tag at '%s'\n", vsp.back ().data ());
      for (auto &&i : children)
        {
          if (i->nd.name == vsp.back ())
            {
              LOG ("got match at '%s'\n", vsp.back ().data ());
              r.push_back (i);
            }
        }

      return r;
    }

  for (Node *&i : children)
    {
      if (i->nd.name == vsp[ti])
        {
          LOG ("got match '%s'\n", vsp[ti].data ());

          std::vector<Node *> p = __eq_rt_dfs_1 (vsp, ti + 1, i->children);

          if (p.size ())
            {
              LOG ("dfs got %lu results from root at '%s'\n", p.size (),
                   vsp[ti].data ());
              r.insert (r.end (), p.begin (), p.end ());
            }
          else
            {
              LOG ("dfs got 0 (p is empty) results from root at '%s'\n",
                   vsp[ti].data ());
            }
        }
    }

  return r;
}
} // namespace equinox
