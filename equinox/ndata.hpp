#if !defined(EQUINOX_NDATA_H)
#define EQUINOX_NDATA_H

#include "header.hpp"

namespace equinox
{
/**
 * Every Node has an associated
 * NodeData structure that stores:
 * 1) Tag Name
 * 2) Attributes
 * 3) Some other metadata
 */
class NodeData
{
public:
  std::string name;
  std::unordered_map<std::string, std::string> attrs;

  bool is_inline_elem;

  NodeData () : is_inline_elem{ false } {}
  NodeData (std::string _Name) : name{ _Name }, is_inline_elem{ false } {}

  inline std::string &
  get_attr (std::string s)
  {
    if (!attrs.count (s))
      throw std::invalid_argument ("NodeData: no attribute '" + s + "'");

    return attrs[s];
  }

  inline void
  set_attr (std::string k, std::string v)
  {
    attrs[k] = v;
  }

  void print ();

  ~NodeData () {}
};
} // namespace equinox

#endif // EQUINOX_NDATA_H
