#if !defined(SOLSTICE_EXPR_HPP)
#define SOLSTICE_EXPR_HPP

#include "const.hpp"
#include "header.hpp"

namespace solstice
{
enum class ExprType
{
  EXPR_CONST,
  EXPR_VAR
};

class Expression
{
  ExprType type;

public:
  explicit Expression (ExprType _Type) : type{ _Type } {}
  virtual ~Expression () {}

  inline const ExprType &
  get_type ()
  {
    return type;
  }

  inline const ExprType &
  get_type () const
  {
    return type;
  }
};

class expr_const : public Expression
{
public:
  Constant *v;

  explicit expr_const (Constant *_V)
      : Expression (ExprType::EXPR_CONST), v{ _V }
  {
  }

  ~expr_const () {}
};

class expr_var : public Expression
{
public:
  sol_string name;

  explicit expr_var (sol_string _Name)
      : Expression (ExprType::EXPR_VAR), name{ _Name }
  {
  }

  ~expr_var () {}
};

void print_expr (Expression *);
} // namespace solstice

#endif // SOLSTICE_EXPR_HPP
