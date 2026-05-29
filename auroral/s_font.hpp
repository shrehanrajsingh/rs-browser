#if !defined(AURORAL_S_FONT_H)
#define AURORAL_S_FONT_H

#include "s_base.hpp"

namespace auroral
{

class auroral_s_font : public auroral_s_base
{
public:
  std::string family;
  float font_size;

  auroral_s_font () : auroral_s_base (AURORAL_S_FONT) {}
  ~auroral_s_font () {}
};
} // namespace auroral

#endif // AURORAL_S_FONT_H
