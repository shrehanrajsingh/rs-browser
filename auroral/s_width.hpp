#if !defined(AURORAL_S_WIDTH_H)
#define AURORAL_S_WIDTH_H

#include "s_base.hpp"

namespace auroral
{

class auroral_s_width : public auroral_s_base
{
public:
  int w;
  auroral_s_width () : auroral_s_base (AURORAL_S_WIDTH), w{ 0 } {}
  ~auroral_s_width () {}
};
} // namespace auroral

#endif // AURORAL_S_WIDTH_H
