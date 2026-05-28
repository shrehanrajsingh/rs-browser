#if !defined(EQUINOX_PARSER_HPP)
#define EQUINOX_PARSER_HPP

#include "header.hpp"
#include "node.hpp"

namespace equinox
{
/**
 * @brief Builds and queries an HTML parse tree from raw markup.
 */
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
  Node *_parser_make_text_tag ();

  int err;

public:
  enum
  {
    EQ_HP_INVALID_SYNTAX = 1,
    EQ_HP_NO_CLOSING_BRACKET_FOUND = 2,
    EQ_HP_INVALID_CLOSING_TAG = 3,
    EQ_HP_NO_HTML_TAG = 4,
  };

  /**
   * @brief Construct an empty parser with no input loaded.
   */
  HTMLParser () : root{ nullptr }, rdp{ nullptr }, end{ false }, err{ 0 } {}
  /**
   * @brief Construct a parser initialized with raw HTML input.
   * @param _Raw_Data HTML markup to parse.
   */
  HTMLParser (std::string _Raw_Data)
      : root{ nullptr }, rdp{ nullptr }, end{ false }, err{ 0 },
        raw_data{ _Raw_Data }
  {
  }

  /**
   * @brief Parse the current input and build the tree rooted at `<html>`.
   */
  void build_tree ();

  /**
   * @brief Check whether a parse error was recorded.
   * @return True if an error occurred while parsing.
   */
  inline bool has_error ();
  /**
   * @brief Access the current parser error code.
   * @return Reference to the error code value.
   */
  inline int &get_error ();

  /**
   * hp.get_tag_fromname ("h1");
   */
  /**
   * @brief Find all nodes by tag name.
   * @param Tag name to search for.
   * @return Vector of matching nodes (may be empty).
   */
  std::vector<Node *> get_tag_fromname (std::string);
  /**
   * hp.get_tag_fromstr ("html body h1");
   */
  /**
   * @brief Find nodes by a whitespace-delimited tag path.
   * @param Tag path string (e.g. "html body h1").
   * @return Vector of matching nodes (may be empty).
   */
  std::vector<Node *> get_tag_fromstr (std::string);

  /**
   * @brief Get the root node of the parsed tree.
   * @return Root node pointer, or nullptr if not built.
   */
  inline Node *
  get_tree ()
  {
    return root;
  }

  /**
   * @brief Destroy the parser and release the tree.
   */
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
