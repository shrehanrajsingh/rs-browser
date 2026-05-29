#if !defined(AURORAL_S_DISPLAY_H)
#define AURORAL_S_DISPLAY_H

#include "s_base.hpp"

namespace auroral
{
enum
{
  AURORAL_S_DISPLAY_TYPE_BLOCK = 0,
  AURORAL_S_DISPLAY_TYPE_FLEX = 1,
  AURORAL_S_DISPLAY_TYPE_INLINE = 2,
};

class auroral_s_display : public auroral_s_base
{
public:
  int disp_type;

  auroral_s_display ()
      : auroral_s_base (AURORAL_S_DISPLAY),
        disp_type{ AURORAL_S_DISPLAY_TYPE_BLOCK }
  {
  }

  ~auroral_s_display () {}
};
} // namespace auroral

#endif // AURORAL_S_DISPLAY_H
