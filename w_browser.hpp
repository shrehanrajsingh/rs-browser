#if !defined(W_BROWSER_H)
#define W_BROWSER_H

#include "header.hpp"
#include "v_browser.hpp"

class QTabWidget;

namespace rs
{
class w_browser : public QMainWindow
{
private:
  QTabWidget *m_tabs;

public:
  w_browser ();
  ~w_browser () = default;

  void add_tab ();
};
} // namespace rs

#endif // W_BROWSER_H
