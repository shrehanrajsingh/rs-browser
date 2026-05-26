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
HTMLParser::build_tree ()
{
  _skip_whitespaces ();

  while (!_parser_stop ())
    {
    }
}
} // namespace equinox
