#include "equinox.hpp"

const std::string html = R"(
    <html>
        <head>
            <title id=1></title>
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
  hp.get_tree ()->print ();

  return 0;
}