#if !defined(SOLSTICE_OBJ_HPP)
#define SOLSTICE_OBJ_HPP

#include "const.hpp"
#include "expr.hpp"
#include "header.hpp"
#include "stmt.hpp"
#include "types.hpp"

namespace solstice
{
enum class ObjType
{
  OBJ_CONST,
};

class Object
{
private:
  ObjType type;

public:
  explicit Object (ObjType _Type) : type{ _Type } {}
  virtual ~Object () {}

  inline const ObjType &
  get_type ()
  {
    return type;
  }
};

class obj_const : public Object
{
public:
  Constant *c;

  explicit obj_const (Constant *_C) : Object (ObjType::OBJ_CONST), c{ _C } {}
  ~obj_const () {}
};
} // namespace solstice

#endif // SOLSTICE_OBJ_HPP
