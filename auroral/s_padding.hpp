#if !defined(AURORAL_S_PADDING_H)
#define AURORAL_S_PADDING_H

#include "s_base.hpp"

namespace auroral
{

class auroral_s_padding : public auroral_s_base
{
public:
  int t;
  int r;
  int b;
  int l;

  auroral_s_padding ()
      : auroral_s_base (AURORAL_S_PADDING), t{ 0 }, r{ 0 }, b{ 0 }, l{ 0 }
  {
  }

  ~auroral_s_padding () {}
};
} // namespace auroral

#endif // AURORAL_S_PADDING_H
