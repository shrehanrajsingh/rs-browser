#if !defined(SOLSTICE_STMT_HPP)
#define SOLSTICE_STMT_HPP

#include "expr.hpp"
#include "header.hpp"
#include "types.hpp"

namespace solstice
{
enum class StmtType
{
  STMT_VARDECL,
  STMT_FUNCALL,
  STMT_IFBLOCK,
  STMT_FUNDECL,
};

class Statement
{
  StmtType type;

public:
  explicit Statement (StmtType _Type) : type{ _Type } {}
  virtual ~Statement () {}
};

class stmt_vardecl : public Statement
{
public:
  sol_string name;
  Expression *val;

  explicit stmt_vardecl (sol_string _Name = sol_string{ "" },
                         Expression *_Val = nullptr)
      : Statement (StmtType::STMT_VARDECL), name{ _Name }, val{ _Val }
  {
  }

  ~stmt_vardecl () {}
};

class stmt_funcall : public Statement
{
public:
  Expression *name;
  sol_vec<Expression *> args;

  explicit stmt_funcall (Expression *_Name = nullptr,
                         sol_vec<Expression *> _Args = sol_vec<Expression *>{})
      : Statement (StmtType::STMT_FUNCALL), name{ _Name }, args{ _Args }
  {
  }

  ~stmt_funcall () {}
};

class stmt_ifblock : public Statement /* no else for now */
{
public:
  Expression *cond;
  sol_vec<Statement *> body;

  explicit stmt_ifblock (Expression *_Cond = nullptr,
                         sol_vec<Statement *> _Body = sol_vec<Statement *>{})
      : Statement (StmtType::STMT_IFBLOCK), cond{ _Cond }, body{ _Body }
  {
  }

  ~stmt_ifblock () {}
};

class stmt_fundecl : public Statement
{
public:
  sol_string name;
  sol_vec<sol_string> args; /* no default values for now, I'm trying to keep it
                               as simple as possible */

  sol_vec<Statement *> body;

  explicit stmt_fundecl (sol_string _Name = sol_string{ "" },
                         sol_vec<sol_string> _Args = sol_vec<sol_string>{},
                         sol_vec<Statement *> _Body = {})
      : Statement (StmtType::STMT_FUNDECL), name{ _Name }, args{ _Args },
        body{ _Body }
  {
  }

  ~stmt_fundecl () {}
};
} // namespace solstice

#endif // SOLSTICE_STMT_HPP
