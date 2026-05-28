#if !defined(EQUINOX_NDATA_H)
#define EQUINOX_NDATA_H

#include "header.hpp"

namespace equinox
{

/**
 * @brief Stores tag name, attributes, and inline metadata.
 */
class NodeData
{
public:
  std::string name;
  std::unordered_map<std::string, std::string> attrs;

  bool is_inline_elem;

  /**
   * @brief Construct empty node data.
   */
  NodeData () : is_inline_elem{ false } {}
  /**
   * @brief Construct node data with a tag name.
   * @param _Name Tag name.
   */
  NodeData (std::string _Name) : name{ _Name }, is_inline_elem{ false } {}

  /**
   * @brief Get an attribute value by key.
   * @param s Attribute key.
   * @return Reference to the attribute value.
   * @throws std::invalid_argument When the attribute is missing.
   */
  inline std::string &
  get_attr (std::string s)
  {
    if (!attrs.count (s))
      throw std::invalid_argument ("NodeData: no attribute '" + s + "'");

    return attrs[s];
  }

  /**
   * @brief Set or overwrite an attribute.
   * @param k Attribute key.
   * @param v Attribute value.
   */
  inline void
  set_attr (std::string k, std::string v)
  {
    attrs[k] = v;
  }

  /**
   * @brief Print this node's data.
   */
  void print ();

  /**
   * @brief Destroy the node data.
   */
  ~NodeData () {}
};
} // namespace equinox

#endif // EQUINOX_NDATA_H
