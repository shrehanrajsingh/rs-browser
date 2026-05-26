#include "node.hpp"

namespace equinox
{
static void
Node::add_next (Node *n, Node *p)
{
  while (n->next != nullptr)
    n = n->next;

  n->next = p;
}
} // namespace equinox
