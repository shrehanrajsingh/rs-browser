#if !defined(EQUINOX_NODE_H)
#define EQUINOX_NODE_H

#include "header.hpp"
#include "ndata.hpp"

namespace equinox
{
/**
 * @brief Node in a linked structure with parent/child relationships.
 *
 * Stores a pointer to the next node in a list, a pointer to the parent
 * node, associated data, and a collection of child nodes.
 */
class Node
{
public:
  Node *next;
  Node *parent;
  NodeData nd;

  std::vector<Node *> children;

  Node () : next{ nullptr }, parent{ nullptr } {}
  Node (Node *_Next) : next{ nullptr }, parent{ nullptr } {}

  static void add_next (Node *, Node *);

  void print (bool _Print_Parent = false);
  std::string get_text ();

  ~Node ()
  {
    if (next != nullptr)
      delete next;
  }
};
} // namespace equinox

#endif // EQUINOX_NODE_H
