#if !defined(V_BROWSER_H)
#define V_BROWSER_H

#include "header.hpp"

namespace rs
{
class __v_browser_meta
{
public:
  int height;
  int width;

  __v_browser_meta () {}
  ~__v_browser_meta () {}
};

/**
 * @brief Qt widget that renders a view from an HTML parse tree.
 */
class v_browser : public QWidget
{
private:
  int m_id;
  equinox::HTMLParser &tree;
  __v_browser_meta *meta = nullptr;

public:
  /**
   * @brief Construct a view browser widget.
   * @param id Instance id.
   * @param _Tree HTML parser providing the tree to render.
   * @param parent Optional parent widget.
   */
  v_browser (int id, equinox::HTMLParser &_Tree,
             __v_browser_meta *_Meta = nullptr, QWidget *parent = nullptr);

  /**
   * @brief Destroy the widget.
   */
  ~v_browser () = default;

  /**
   * @brief Build a widget subtree from a parsed node.
   * @param Node Root node for this widget subtree.
   * @return Newly created widget.
   */
  QWidget *build_widget (equinox::Node *);

protected:
  // void paintEvent (QPaintEvent *) override;
};

} // namespace rs

#endif // V_BROWSER_H
