#if !defined(EQUINOX_NDATA_H)
#define EQUINOX_NDATA_H

#include "header.hpp"

namespace equinox
{
class NodeData
{
public:
  std::string name;
  std::unordered_map<std::string, std::string> attrs;

  NodeData () {}
  NodeData (std::string _Name) : name{ _Name } {}

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

  ~NodeData () {}
};
} // namespace equinox

#endif // EQUINOX_NDATA_H
