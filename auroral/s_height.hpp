#if !defined(AURORAL_S_HEIGHT_H)
#define AURORAL_S_HEIGHT_H

#include "s_base.hpp"

namespace auroral
{

class auroral_s_height : public auroral_s_base
{
public:
  int h;
  auroral_s_height () : auroral_s_base (AURORAL_S_HEIGHT), h{ 0 } {}
  ~auroral_s_height () {}
};
} // namespace auroral

#endif // AURORAL_S_HEIGHT_H
