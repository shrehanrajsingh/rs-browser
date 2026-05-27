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
Node::print (bool pp)
{
  nd.print ();

  if (pp)
    {
      if (parent != nullptr)
        {
          std::cout << "parent:\n";
          parent->print ();
        }
      else
        std::cout << "parent: (nullptr)\n";
    }
}

std::string
Node::get_text ()
{
  for (auto &&i : children)
    {
      if (i->nd.name == "[text]")
        return i->nd.get_attr ("_");
    }

  return std::string{ "" };
}
} // namespace equinox
