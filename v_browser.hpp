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

  unsigned long long RAND_COUNT = 0;

  QVBoxLayout *rt_layout;
  QWidget *page_content;

  QTimer *timer;

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

  /**
   * @brief Populate a box layout with a node's children.
   *
   * Consecutive inline elements (img/input/label/button) are grouped into a
   * shared horizontal row. The stretch parameters model CSS flex
   * justify-content spacing (leading/trailing/between stretches), while
   * @p item_align controls per-item cross-axis alignment.
   */
  void populate_layout (QBoxLayout *layout, equinox::Node *n,
                        Qt::Alignment item_align, int lead_stretch,
                        int trail_stretch, int between_stretch);

  /**
   * @brief Apply CSS width/height (px, vw, vh) as fixed dimensions.
   */
  void apply_box_dimensions (QWidget *container, const std::string &width,
                             const std::string &height);

  void relayout_page ();

  void resizeEvent (QResizeEvent *);

protected:
  // void paintEvent (QPaintEvent *) override;
};

} // namespace rs

#endif // V_BROWSER_H
