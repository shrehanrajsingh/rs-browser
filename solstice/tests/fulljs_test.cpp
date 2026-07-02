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

  solstice::vm_t *vm = new solstice::vm_t;
  solstice::codegen (vm, v);

  std::cout << std::endl;
  for (solstice::bytecode_t &i : vm->c)
    {
      solstice::print_bytecode (i);
    }

  std::cout << std::endl;
  for (solstice::Constant *&i : vm->table_const)
    {
      solstice::print_constant (i);
    }

  vm->execute ();

  f.close ();
  return 0;
}