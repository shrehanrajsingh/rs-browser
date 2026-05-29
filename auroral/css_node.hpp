#if !defined(AURORAL_CSS_NODE_H)
#define AURORAL_CSS_NODE_H

#include "header.hpp"

namespace auroral
{
class css_node
{
public:
  std::unordered_map<std::string, std::variant<std::string, css_node *>> mp;

  css_node () {}

  void add_pair (std::string, std::string);
  void add_pair (std::string, css_node *);

  /* value of a key is a string */
  bool has_key (std::string);
  bool val_is_str (std::string);
  std::string get_ifstr (std::string);
  css_node *get_ifnode (std::string);

  ~css_node () {}
};
} // namespace auroral

#endif // AURORAL_CSS_NODE_H
