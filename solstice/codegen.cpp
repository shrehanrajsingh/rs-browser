#include "codegen.hpp"

namespace solstice
{
void
print_bytecode (bytecode_t &b)
{
  const ByteCodeType &t = b.get_type ();
  int a = b.get_a ();
  int bb = b.get_b ();
  char *c = b.get_c ();

  switch (t)
    {
    case ByteCodeType::OP_LOAD_CONST:
      std::cout << "OP_LOAD_CONST";
      break;
    case ByteCodeType::OP_STORE_GLOBAL:
      std::cout << "OP_STORE_GLOBAL";
      break;
    case ByteCodeType::OP_STORE_FRAME:
      std::cout << "OP_STORE_FRAME";
      break;
    case ByteCodeType::OP_LOAD_NAME:
      std::cout << "OP_LOAD_NAME";
      break;
    default:
      std::cout << "UNKNOWN_OP";
      break;
    }

  std::cout << " a=" << a << " b=" << bb;
  if (c)
    std::cout << " c=\"" << c << "\"";
  else
    std::cout << " c=nullptr";
  std::cout << std::endl;
}
} // namespace solstice
