#include "css_node.hpp"

namespace auroral
{
void
css_node::add_pair (std::string s, std::string v)
{
  mp[s] = v;
}

void
css_node::add_pair (std::string s, css_node *n)
{
  mp[s] = n;
}

bool
css_node::has_key (std::string s)
{
  return !!mp.count (s);
}

bool
css_node::val_is_str (std::string v)
{
  return std::holds_alternative<std::string> (mp[v]);
}

std::string
css_node::get_ifstr (std::string v)
{
  return std::get<std::string> (mp[v]);
}

css_node *
css_node::get_ifnode (std::string v)
{
  return std::get<css_node *> (mp[v]);
}

} // namespace auroral
