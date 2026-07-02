#if !defined(SOLSTICE_CODEGEN_HPP)
#define SOLSTICE_CODEGEN_HPP

#include "const.hpp"
#include "expr.hpp"
#include "header.hpp"
#include "stmt.hpp"
#include "types.hpp"

namespace solstice
{
/**
 * OP_LOAD_CONST: a -> entry in table_const
 * OP_STORE_GLOBAL: a -> slot number
 * OP_STORE_LOCAL: a -> slot number
 * OP_LOAD_NAME: a -> slot number, b -> 0 (global), 1 (frame)
 * OP_RETURN: 0, 0, nullptr
 * OP_JUMP: a -> index to jump (0-indexed)
 * OP_STOREFUNC_GLOBAL: a -> slot number, b -> ip
 */
enum class ByteCodeType
{
  OP_LOAD_CONST,       /* load a const from table_const and push to stack */
  OP_STORE_GLOBAL,     /* store var globally */
  OP_STORE_LOCAL,      /* store var in frame (locals) */
  OP_LOAD_NAME,        /* read a var and push it's value to stack */
  OP_RETURN,           /* pop top ip */
  OP_JUMP,             /* jump to bytecode address (0-indexed) */
  OP_STOREFUNC_GLOBAL, /* store function in globals */
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
