#include "css_scale.hpp"

namespace auroral
{

css_scale_unit
css_scale_unit::parse_type (std::string s)
{
  /* get last two digits for unit */
  char l1 = s.back ();
  char l2 = s[s.size () - 2];

  css_scale_unit cu;
  cu.value = std::stoi (s.substr (0, s.size () - 2));

  if (l2 == 'p' && l1 == 'x')
    {
      cu.type = AURORAL_CSS_SCALE_PX;
    }
  else if (l2 == 'v' && l1 == 'w')
    {
      cu.type = AURORAL_CSS_SCALE_VIEWPORT_WIDTH;
    }
  else if (l2 == 'v' && l1 == 'h')
    {
      cu.type = AURORAL_CSS_SCALE_VIEWPORT_HEIGHT;
    }

  return cu;
}

} // namespace auroral
