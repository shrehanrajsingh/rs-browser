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
  NodeData nd;

  Node () : next{ nullptr } {}
  Node (Node *_Next) : next{ nullptr } {}

  static void add_next (Node *, Node *);

  ~Node ()
  {
    if (next != nullptr)
      delete next;
  }
};
} // namespace equinox

#endif // EQUINOX_NODE_H
