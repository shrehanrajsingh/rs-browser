#if !defined(AURORAL_S_BASE_H)
#define AURORAL_S_BASE_H

#include "header.hpp"

namespace auroral
{
enum
{
  AURORAL_S_DISPLAY = 1,
  AURORAL_S_HEIGHT = 2,
  AURORAL_S_WIDTH = 3,
  AURORAL_S_FONT = 4,
  AURORAL_S_PADDING = 5,
};

class auroral_s_base
{
public:
  int type;

  auroral_s_base () : type{ 0 } {}
  auroral_s_base (int _T) : type{ _T } {}
  virtual ~auroral_s_base () {}
};
} // namespace auroral

#endif // AURORAL_S_BASE_H
