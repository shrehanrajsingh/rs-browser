#if !defined(AURORAL_PARSER_H)
#define AURORAL_PARSER_H

#include "css_node.hpp"
#include "header.hpp"

namespace auroral
{
class CSSParser
{
public:
  css_node *nodes;

  CSSParser () { nodes = new css_node (); }
  ~CSSParser () { delete nodes; }

  static CSSParser parse_inline (std::string_view);
};
} // namespace auroral

#endif // AURORAL_PARSER_H
