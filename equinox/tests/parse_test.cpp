#include "equinox.hpp"

const std::string html = R"(
    <html>
        <head>
            <title id=1>Hello</title>
        </head>

        <body>
            <h1>Hello</h1>
        </body>
    </html>
)";

int
main (int argc, char const *argv[])
{
  using namespace equinox;

  HTMLParser hp (html);
  hp.build_tree ();
  Node *n = hp.get_tree ();

  while (n != nullptr)
    {
      std::cout << n->nd.name << '\t' << n->children.size () << '\n';
      n = n->next;
    }

  std::vector<Node *> h = hp.get_tag_fromstr ("html body h1");

  for (auto &&i : h)
    {
      i->print ();
      std::cout << "------------------------\n";
    }

  return 0;
}