#if !defined(AURORAL_CSS_SCALE_H)
#define AURORAL_CSS_SCALE_H

#include "header.hpp"

namespace auroral
{
enum
{
  AURORAL_CSS_SCALE_PX = 0,
  AURORAL_CSS_SCALE_VIEWPORT_WIDTH = 1,
  AURORAL_CSS_SCALE_VIEWPORT_HEIGHT = 2,
};

class css_scale_unit
{
public:
  int type;
  int value;

  css_scale_unit () : type{ AURORAL_CSS_SCALE_PX }, value{ 0 } {}
  css_scale_unit (int _Type) : type{ _Type }, value{ 0 } {}
  css_scale_unit (int _Type, int _Value) : type{ _Type }, value{ _Value } {}

  static css_scale_unit parse_type (std::string);

  ~css_scale_unit () {}
};
} // namespace auroral

#endif // AURORAL_CSS_SCALE_H
