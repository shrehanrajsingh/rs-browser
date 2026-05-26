#if !defined(EQUINOX_PARSER_HPP)
#define EQUINOX_PARSER_HPP

#include "header.hpp"
#include "node.hpp"

namespace equinox
{
class HTMLParser
{
private:
  Node *root; /* <html>...</html> */
  std::string raw_data;
  char *rdp; /* raw data pointer */
  bool end;

  void _skip_whitespaces ();
  inline bool &_parser_stop ();

public:
  HTMLParser () : root{ nullptr }, rdp{ nullptr }, end{ false } {}

  void build_tree ();

  ~HTMLParser ()
  {
    if (root != nullptr)
      {
        delete root;
      }
  }
};
} // namespace equinox

#endif // EQUINOX_PARSER_HPP
