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
  OBJ_FUNC,
};

class Object
{
private:
  ObjType type;
  size_t count;

  //   std::mutex clk;

public:
  explicit Object (ObjType _Type) : type{ _Type }, count{ 0 } {}
  virtual ~Object () {}

  inline const ObjType &
  get_type ()
  {
    return type;
  }

  inline const size_t &
  get_count ()
  {
    return count;
  }

  inline void
  inc_count ()
  {
    count++;
  }

  inline void
  dec_count ()
  {
    if (count)
      count--;
  }
};

class obj_const : public Object
{
public:
  Constant *c;

  explicit obj_const (Constant *_C) : Object (ObjType::OBJ_CONST), c{ _C } {}
  ~obj_const () {}
};

class obj_func : public Object
{
public:
  sol_string name;
  size_t ip;

  explicit obj_func (sol_string _Name, size_t _Ip)
      : Object (ObjType::OBJ_FUNC), name{ _Name }, ip{ _Ip }
  {
  }

  ~obj_func () {}
};
} // namespace solstice

#endif // SOLSTICE_OBJ_HPP
