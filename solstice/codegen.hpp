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
  OP_STORE_GLOBAL, /* store var globally */
  OP_STORE_FRAME,  /* store var in frame (locals) */
  OP_LOAD_NAME,
};

class bytecode_t
{
  ByteCodeType type;
  int a;
  int b;
  char *c;

public:
  explicit bytecode_t (ByteCodeType _Type, int _A = 0, int _B = 0,
                       char *_C = nullptr)
      : type{ _Type }, a{ _A }, b{ _B }, c{ _C }
  {
  }

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

void print_bytecode (bytecode_t &);
} // namespace solstice

#endif // SOLSTICE_CODEGEN_HPP
