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

  if (parent != nullptr)
    {
      std::cout << "parent:\n";
      parent->print ();
    }
  else
    std::cout << "parent: (nullptr)\n";
}
} // namespace equinox
