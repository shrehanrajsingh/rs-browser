#include "auroral.hpp"

template <class... Ts> struct overloaded : Ts...
{
  using Ts::operator()...;
};

template <class... Ts> overloaded (Ts...) -> overloaded<Ts...>;

int
main (int argc, char const *argv[])
{
  using namespace auroral;

  CSSParser p = CSSParser::parse_inline (R"(
        display: flex;
        flex-direction: column;
    )");

  for (auto &&i : p.nodes->mp)
    {
      std::cout << i.first << '\n';

      std::visit (overloaded{ [] (std::string arg)
                                { std::cout << "(string) " << arg << '\n'; },
                              [] (css_node *arg)
                                { std::cout << "(css_node)\n"; } },
                  i.second);
    }

  return 0;
}