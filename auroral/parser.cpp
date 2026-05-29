#include "parser.hpp"

namespace auroral
{
CSSParser
CSSParser::parse_inline (std::string_view ss)
{
  CSSParser par;
  char *s = const_cast<char *> (ss.data ());
  char c;

  bool saw_colon = false;
  std::string key = "";
  std::string val = "";

  while ((c = *s++) != '\0')
    {
      if (c == ':')
        {
          while (std::isspace (static_cast<unsigned char> (key.back ())))
            key.pop_back ();

          saw_colon = true;
          continue;
        }

      if (c == ';')
        {
          while (std::isspace (static_cast<unsigned char> (val.back ())))
            val.pop_back ();

          par.nodes->add_pair (key, val);
          key.clear ();
          val.clear ();

          saw_colon = false;
          continue;
        }

      if (saw_colon)
        {
          if (!val.size () && std::isspace (static_cast<unsigned char> (c)))
            continue;

          val.push_back (c);
        }
      else
        {
          if (!key.size () && std::isspace (static_cast<unsigned char> (c)))
            continue;

          key.push_back (c);
        }
    }

  if (key.size () && val.size ())
    {
      while (std::isspace (static_cast<unsigned char> (val.back ())))
        val.pop_back ();

      par.nodes->add_pair (key, val);
    }

  return par;
}
} // namespace auroral
