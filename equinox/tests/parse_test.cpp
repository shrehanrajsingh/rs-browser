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
      std::cout << n->nd.name << '\n';
      n = n->next;
    }

  hp.get_tag_fromstr ("html body h1");

  return 0;
}