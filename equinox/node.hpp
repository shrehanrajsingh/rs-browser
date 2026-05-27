#if !defined(EQUINOX_NODE_H)
#define EQUINOX_NODE_H

#include "header.hpp"
#include "ndata.hpp"

namespace equinox
{
/**
 * Linked list
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

  void print ();

  ~Node ()
  {
    if (next != nullptr)
      delete next;
  }
};
} // namespace equinox

#endif // EQUINOX_NODE_H
