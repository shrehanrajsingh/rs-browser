#include "ndata.hpp"

namespace equinox
{
void
NodeData::print ()
{
  std::cout << "tag_name: " << name << "\n"
            << (is_inline_elem ? "is_inline: true" : "is_inline: false")
            << "\nAttributes: " << attrs.size () << '\n';

  for (auto &i : attrs)
    {
      std::cout << i.first << "=" << i.second << '\n';
    }
}
} // namespace equinox
