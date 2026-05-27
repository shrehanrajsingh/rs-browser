#if !defined(V_BROWSER_H)
#define V_BROWSER_H

#include "header.hpp"

namespace rs
{
class v_browser : public QWidget
{
private:
  int m_id;
  equinox::HTMLParser &tree;

public:
  v_browser (int id, equinox::HTMLParser &_Tree, QWidget *parent = nullptr);
  ~v_browser () = default;

  QWidget *build_widget (equinox::Node *);

protected:
  // void paintEvent (QPaintEvent *) override;
};

} // namespace rs

#endif // V_BROWSER_H
