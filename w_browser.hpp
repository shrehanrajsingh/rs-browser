#if !defined(W_BROWSER_H)
#define W_BROWSER_H

#include "header.hpp"
#include "v_browser.hpp"

class QTabWidget;

namespace rs
{
/**
 * @brief Main window hosting tabbed browser views.
 */
class w_browser : public QMainWindow
{
private:
  QTabWidget *m_tabs;

  equinox::HTMLParser new_tab_html;
  __v_browser_meta *vbm = nullptr;

public:
  /**
   * @brief Construct the main browser window.
   */
  w_browser ();
  /**
   * @brief Destroy the main window.
   */
  ~w_browser () = default;

  /**
   * @brief Add a new browser tab.
   */
  void add_tab ();
  void resizeEvent (QResizeEvent *);
};
} // namespace rs

#endif // W_BROWSER_H
