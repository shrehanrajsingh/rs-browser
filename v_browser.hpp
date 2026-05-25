#if !defined(V_BROWSER_H)
#define V_BROWSER_H

#include "header.hpp"

namespace rs
{
class v_browser : public QWidget
{
private:
  int m_id;

public:
  v_browser (int id, QWidget *parent = nullptr);
  ~v_browser () = default;

protected:
  // void paintEvent (QPaintEvent *) override;
};

} // namespace rs

#endif // V_BROWSER_H
