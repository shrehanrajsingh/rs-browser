#if !defined(SOLSTICE_CONST_HPP)
#define SOLSTICE_CONST_HPP

#include "header.hpp"
#include "types.hpp"

namespace solstice
{

enum class ConstType
{
  CONST_INT,
  CONST_FLOAT,
  CONST_STRING,
  CONST_BOOL,
  CONST_UNDEFINED
};

class Constant
{
  ConstType type;

public:
  explicit Constant (ConstType _Type) : type{ _Type } {}
  virtual ~Constant () {}

  inline const ConstType &
  get_type ()
  {
    return type;
  }
};

class const_int : public Constant
{
public:
  sol_int v;

  explicit const_int (sol_int _V) : Constant (ConstType::CONST_INT), v{ _V } {}
  ~const_int () {}
};

class const_float : public Constant
{
public:
  float v;

  explicit const_float (float _V) : Constant (ConstType::CONST_FLOAT), v{ _V }
  {
  }

  ~const_float () {}
};

class const_string : public Constant
{
public:
  sol_string v;

  explicit const_string (sol_string _V)
      : Constant (ConstType::CONST_STRING), v{ _V }
  {
  }

  ~const_string () {}
};

class const_bool : public Constant
{
public:
  bool v;

  explicit const_bool (bool _V) : Constant (ConstType::CONST_BOOL), v{ _V } {}
  ~const_bool () {}
};

class const_undefined : public Constant
{
public:
  explicit const_undefined () : Constant (ConstType::CONST_UNDEFINED) {}
  ~const_undefined () {}
};

void print_constant (Constant *);
} // namespace solstice

#endif // SOLSTICE_CONST_HPP
