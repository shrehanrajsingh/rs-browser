#include "node.hpp"

namespace equinox
{
void
Node::add_next (Node *n, Node *p)
{
  while (n->next != nullptr)
    n = n->next;

  n->next = p;
}

void
Node::print ()
{
  nd.print ();

  if (next != nullptr)
    next->print ();
  else
    std::cout << "(nullptr)\n";
}
} // namespace equinox
