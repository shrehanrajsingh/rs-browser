#if !defined(SOLSTICE_CODEGEN_HPP)
#define SOLSTICE_CODEGEN_HPP

#include "const.hpp"
#include "expr.hpp"
#include "header.hpp"
#include "stmt.hpp"
#include "types.hpp"

namespace solstice
{
enum class ByteCodeType
{
  OP_LOAD_CONST,
  OP_STORE_GLOBAL,
  OP_STORE_FRAME,
  OP_LOAD_NAME,
};

class bytecode_t
{
  ByteCodeType type;
  int a;
  int b;
  char *c;

public:
  bytecode_t () {}
  ~bytecode_t () {}

  inline const ByteCodeType &
  get_type ()
  {
    return type;
  }

  inline int &
  get_a ()
  {
    return a;
  }

  inline int &
  get_b ()
  {
    return b;
  }

  inline char *&
  get_c ()
  {
    return c;
  }
};
} // namespace solstice

#endif // SOLSTICE_CODEGEN_HPP
