#if !defined(SOLSTICE_AST_HPP)
#define SOLSTICE_AST_HPP

#include "expr.hpp"
#include "header.hpp"
#include "stmt.hpp"
#include "types.hpp"

namespace solstice
{
sol_vec<Statement *> make_ast (sol_string);
} // namespace solstice

#endif // SOLSTICE_AST_HPP
