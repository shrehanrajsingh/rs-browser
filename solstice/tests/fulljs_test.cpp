#include "solstice.hpp"

int
main (int argc, char const *argv[])
{
  std::ifstream f ("../solstice/tests/test.js");

  if (!f.is_open ())
    {
      fprintf (stderr, "error, cannot open file\n");
      exit (-1);
    }

  std::ostringstream ss;
  ss << f.rdbuf ();

  std::string fc = ss.str ();

  auto v = solstice::make_ast (fc);

  for (auto &&i : v)
    solstice::print_statement (i);

  f.close ();
  return 0;
}