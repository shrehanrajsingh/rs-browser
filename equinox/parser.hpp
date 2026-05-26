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

  inline void _init_root ();

  void _set_ptrs ();
  void _skip_whitespaces ();

  inline bool &_parser_stop ();
  bool _parser_next (char *);
  char *_parser_snapshot ();

  /**
   * Go to end of bracket.
   * After this, `rdp` will point to
   * either \0 (syntax error/no bracket found) or the next
   * character after '>'
   */
  void _parser_goto_endofbracket ();
  void _parser_parse_attr (Node *);

  Node *_parser_maketag_withattrs ();

  int err;

public:
  static enum {
    EQ_HP_INVALID_SYNTAX = 1,
    EQ_HP_NO_CLOSING_BRACKET_FOUND = 2,
    EQ_HP_INVALID_CLOSING_TAG = 3,
    EQ_HP_NO_HTML_TAG = 4,
  };

  HTMLParser () : root{ nullptr }, rdp{ nullptr }, end{ false }, err{ 0 } {}
  HTMLParser (std::string _Raw_Data)
      : root{ nullptr }, rdp{ nullptr }, end{ false }, err{ 0 }
  {
  }

  void build_tree ();

  inline bool has_error ();
  inline int &get_error ();

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
