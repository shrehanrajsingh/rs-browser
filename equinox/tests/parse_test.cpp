#include "equinox.hpp"

const std::string html = R"(
    <html>
        <head>
            <title id=1 style="background: red;">Hello</title>
        </head>

        <body>
            <h1>Hello</h1>
            <h1>Hi</h1>

            <img src="../../scene.jpg" alt="Alt Text" />
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
      n->print ();
      std::cout << n->nd.name << '\t' << n->children.size () << '\n';
      n = n->next;
    }

  std::vector<Node *> h = hp.get_tag_fromstr ("html body h1 [text]");

  for (auto &&i : h)
    {
      i->print ();
      std::cout << "------------------------\n";
    }

  return 0;
}