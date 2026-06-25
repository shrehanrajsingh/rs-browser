#include "v_browser.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace rs
{
std::string
_str_trim (std::string v)
{
  size_t i = 0;

  while (std::isspace (static_cast<unsigned char> (v[i])))
    i++;

  if (i == v.size ())
    return std::string{ "" };

  while (std::isspace (static_cast<unsigned char> (v.back ())))
    v.pop_back ();

  return v.substr (i);
}

static int
_count_grid_tracks (const std::string &spec)
{
  if (spec.empty ())
    return 1;

  size_t rp = spec.find ("repeat");
  if (rp != std::string::npos)
    {
      size_t lp = spec.find ('(', rp);
      size_t comma = spec.find (',', lp);

      if (lp != std::string::npos && comma != std::string::npos)
        {
          try
            {
              int v = std::stoi (
                  _str_trim (spec.substr (lp + 1, comma - lp - 1)));
              return v > 0 ? v : 1;
            }
          catch (const std::exception &e)
            {
            }
        }
    }

  int count = 0;
  std::istringstream iss (spec);
  std::string tok;
  while (iss >> tok)
    count++;

  return count > 0 ? count : 1;
}

v_browser::v_browser (int id, equinox::HTMLParser &_Tree,
                      __v_browser_meta *_Meta, QWidget *parent)
    : QWidget (parent), m_id (id), tree{ _Tree }, meta{ _Meta }
{
  rt_layout = new QVBoxLayout (this);

  timer = new QTimer (this);

  timer->setSingleShot (true);

  connect (timer, &QTimer::timeout, this, &v_browser::relayout_page);

  rt_layout->setContentsMargins (0, 0, 0, 0);
  rt_layout->setSpacing (0);

  auto *top_bar = new QWidget ();

  top_bar->setStyleSheet ("background: #666;");

  auto *top_layout = new QHBoxLayout (top_bar);

  auto *btn_back = new QPushButton ("←");
  auto *btn_frwd = new QPushButton ("→");
  auto *btn_refresh = new QPushButton ("⟳");
  auto *address_bar = new QLineEdit ();

  const QString btn_style_tl = R"(
      QPushButton {
        font-size: 20px;
        padding: 0px;
        margin: 0px;
        border: none;
        background: transparent;
      }
      QPushButton:hover {
        background: #555;
      }
    )";

  btn_back->setStyleSheet (btn_style_tl);
  btn_frwd->setStyleSheet (btn_style_tl);
  btn_refresh->setStyleSheet (R"(
      QPushButton {
        font-size: 24px;
        padding: 0px;
        margin: 0px;
        border: none;
        background: transparent;
      }
      QPushButton:hover {
        background: #555;
        border-radius: 4px;
      }
    )");

  btn_back->setFixedSize (28, 28);
  btn_frwd->setFixedSize (28, 28);
  btn_refresh->setFixedSize (28, 28);

  address_bar->setPlaceholderText ("Search or enter address");
  address_bar->setStyleSheet ("color: white; padding: 5px 3px 5px 8px;");

  top_layout->addWidget (btn_back, 1);
  top_layout->addWidget (btn_frwd, 1);
  top_layout->addWidget (btn_refresh, 1);
  top_layout->addWidget (address_bar, 37);

  page_content = build_widget (tree.get_tag_fromname ("body")[0]);

  // page_content->setAlignment (Qt::AlignCenter);

  rt_layout->addWidget (top_bar, 1);
  rt_layout->addWidget (page_content, 15);
}

QWidget *
v_browser::build_widget (equinox::Node *n)
{
  std::string &tn = n->nd.name;

  if (tn == "body")
    {
      auto *container = new QWidget ();

      container->setAutoFillBackground (true);
      container->setAttribute (Qt::WA_StyledBackground, true);

      container->setStyleSheet (R"(
          padding: 0;
          margin: 0;
          color: black;
          font-size: 32px;
        )");

      std::string attr_style = "";

      try
        {
          attr_style = n->nd.get_attr ("style");
        }
      catch (const std::exception &e)
        {
        }

      auto *layout = new QVBoxLayout (container);

      if (attr_style.size ())
        {
          LOG ("body style = %s\n", attr_style.data ());
          unsigned long long rcc = RAND_COUNT++;
          std::string rid = "__rsb_Body_TAG_" + std::to_string (rcc);

          container->setObjectName (rid);

          container->setStyleSheet (
              QString::fromStdString ("#" + rid + " {" + attr_style + "}"));
        }

      layout->setAlignment (Qt::AlignTop);

      QWidget *inline_container = nullptr;
      QHBoxLayout *inline_row = nullptr;
      for (auto &i : n->children)
        {
          QWidget *iw = build_widget (i);

          if (i->nd.name == "br")
            {
              if (inline_row)
                {
                  inline_row->addStretch ();
                }

              inline_row = nullptr;
              inline_container = nullptr;
            }

          if (!iw)
            continue;

          if (i->nd.name == "img" || i->nd.name == "input"
              || i->nd.name == "label" || i->nd.name == "button")
            {
              if (!inline_row)
                {
                  inline_container = new QWidget ();

                  inline_container->setSizePolicy (QSizePolicy::Preferred,
                                                   QSizePolicy::Fixed);

                  inline_row = new QHBoxLayout (inline_container);
                  inline_row->setAlignment (Qt::AlignLeft | Qt::AlignTop);
                  inline_row->setSpacing (8);
                  inline_row->setContentsMargins (0, 0, 0, 0);
                  layout->addWidget (inline_container, 0, Qt::AlignLeft);
                }

              inline_row->addWidget (iw, 0, Qt::AlignBottom);
            }
          else
            {
              inline_row = nullptr;
              layout->addWidget (iw);
            }
        }

      return container;
    }

  if (tn == "h1" || tn == "h2" || tn == "h3" || tn == "h4" || tn == "h5"
      || tn == "h6")
    {
      auto *label = new QLabel (QString::fromStdString (n->get_text ()));

      label->setAttribute (Qt::WA_TranslucentBackground);

      if (tn == "h1")
        label->setStyleSheet (R"(
          font-size: 32px;
          font-weight: bold;
        )");
      else if (tn == "h2")
        label->setStyleSheet (R"(
          font-size: 24px;
          font-weight: bold;
        )");
      else if (tn == "h3")
        label->setStyleSheet (R"(
          font-size: 19px;
          font-weight: bold;
        )");
      else if (tn == "h4")
        label->setStyleSheet (R"(
          font-size: 16px;
          font-weight: bold;
        )");
      else if (tn == "h5")
        label->setStyleSheet (R"(
          font-size: 13px;
          font-weight: bold;
        )");
      else if (tn == "h6")
        label->setStyleSheet (R"(
          font-size: 10px;
          font-weight: bold;
        )");

      std::string attr_style = "";

      try
        {
          attr_style = n->nd.get_attr ("style");
        }
      catch (const std::exception &e)
        {
          std::cerr << e.what () << '\n';
        }

      if (attr_style.size ())
        {
          unsigned long long rcc = RAND_COUNT++;
          std::string rid = "__rsb_TAG_" + tn + "_" + std::to_string (rcc);

          label->setObjectName (rid);

          label->setStyleSheet (
              QString::fromStdString ("#" + rid + " {" + attr_style + "}"));
        }

      label->setAlignment (Qt::AlignLeft | Qt::AlignTop);
      label->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Fixed);

      return label;
    }

  if (tn == "p")
    {
      auto *label = new QLabel (QString::fromStdString (n->get_text ()));

      label->setStyleSheet (R"(
          font-size: 16px;
        )");

      label->setAlignment (Qt::AlignLeft | Qt::AlignTop);
      label->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Fixed);

      return label;
    }

  if (tn == "img")
    {
      auto *container = new QWidget ();

      container->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);

      auto *layout = new QVBoxLayout (container);
      layout->setAlignment (Qt::AlignTop);

      layout->setContentsMargins (0, 0, 0, 0);
      layout->setSpacing (0);

      std::string src = "";
      std::string alt = "";

      try
        {
          src = n->nd.get_attr ("src");
        }
      catch (const std::exception &e)
        {
          std::cerr << "src attribute required for img\n";
          exit (EXIT_FAILURE);
        }

      try
        {
          alt = n->nd.get_attr ("alt");
        }
      catch (const std::exception &e)
        {
          /* do nothing */
        }

      auto *label = new QLabel ();
      label->setAlignment (Qt::AlignTop | Qt::AlignLeft);

      auto *alt_label = new QLabel ();

      alt_label->setAlignment (Qt::AlignTop | Qt::AlignLeft);
      alt_label->setStyleSheet (R"(
            color: #666;
            font-size: 14px;
          )");

      if (src[0] == '\"' || src[0] == '\'')
        src = src.substr (1, src.size () - 2);

      if (alt[0] == '\"' || alt[0] == '\'')
        alt = alt.substr (1, alt.size () - 2);

      QString alt_text;

      if (alt.size ())
        alt_text = QString::fromStdString (alt);

      QPixmap pixmap (QString::fromStdString (src));

      if (!pixmap.isNull ())
        {
          label->setPixmap (pixmap.scaled (400, 400, Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));

          alt_label->hide ();
        }
      else
        {
          pixmap = QPixmap (
              QString::fromStdString ("../resources/image_not_found.png"));

          label->setPixmap (pixmap.scaled (200, 200, Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));

          alt_label->setText (alt_text);
        }

      layout->addWidget (label);
      layout->addWidget (alt_label);

      return container;
    }

  if (tn == "input")
    {
      std::string inp_type = "";
      std::string attr_style = "";

      try
        {
          inp_type = n->nd.get_attr ("type");
        }
      catch (const std::exception &e)
        {
          inp_type = "text"; /* fallback */
        }

      try
        {
          attr_style = n->nd.get_attr ("style");
        }
      catch (const std::exception &e)
        {
          std::cerr << e.what () << '\n';
        }

      if (inp_type[0] == '\"' || inp_type[0] == '\'')
        inp_type = inp_type.substr (1, inp_type.size () - 2);

      if (inp_type == "text")
        {
          std::string attr_placeholder = "";

          try
            {
              attr_placeholder = n->nd.get_attr ("placeholder");
            }
          catch (const std::exception &e)
            {
            }

          auto *input = new QLineEdit ();

          if (attr_placeholder.size ())
            {
              input->setPlaceholderText (
                  QString::fromStdString (attr_placeholder));
            }

          input->setStyle (QStyleFactory::create ("Fusion"));

          input->setFrame (false);
          input->setAttribute (Qt::WA_MacShowFocusRect, false);

          input->setMinimumWidth (250);

          if (attr_style.size ())
            {
              unsigned long long rcc = RAND_COUNT++;
              std::string rid = "__rsb_Input_Text_" + std::to_string (rcc);

              input->setObjectName (rid);

              input->setStyleSheet (
                  QString::fromStdString ("#" + rid + " {" + attr_style + R"(
                    ;background-color: white;
                    color: #000;
                    font-size: 14px;
                  }
              )"));
            }

          return input;
        }

      else if (inp_type == "checkbox")
        {
          auto *input = new QCheckBox ();
          input->setMinimumSize (40, 30);

          input->setStyleSheet (R"(
              QCheckBox::indicator {
                  width: 20px;
                  height: 20px;

                  border: 1px solid #888;
                  border-radius: 4px;

                  background: white;
              }

              QCheckBox::indicator:checked {
                  background: #0078d7;

                  image: url(../resources/checkmark.png);
              }
            )");

          return input;
        }

      else if (inp_type == "radio")
        {
          auto *input = new QRadioButton ();
          input->setMinimumSize (40, 30);

          input->setStyleSheet (R"(
              QRadioButton::indicator {
                  width: 20px;
                  height: 20px;

                  border: 1px solid #888;
                  border-radius: 10px;

                  background: white;
              }

              QRadioButton::indicator:checked {
                  background: #0078d7;

                  image: url(../resources/radio.png);
              }
            )");

          return input;
        }

      else if (inp_type == "date")
        {
          auto *input = new QDateEdit ();

          input->setStyleSheet (R"(
            QDateEdit {
              background: rgb(230,230,230);
              color: black;
              border: none;
              border-radius: 8px;
              padding: 6px 10px;
              font-size: 14px;
            }

            QDateEdit::up-button {
                width: 18px;
                border-left: 1px solid #c0c0c0;
                background: #f5f5f5;
                image: url(../resources/up-arrow.png);
            }

            QDateEdit::down-button {
                width: 18px;
                border-left: 1px solid #c0c0c0;
                background: #f5f5f5;
                image: url(../resources/down-arrow.png);
            }

            QDateEdit::up-button:hover,
            QDateEdit::down-button:hover {
                background: #e5e5e5;
            }
          )");

          return input;
        }
      else if (inp_type == "datetime")
        {
          auto *input = new QDateTimeEdit ();

          input->setStyleSheet (R"(
            QDateTimeEdit {
              background: rgb(230,230,230);
              color: black;
              border: none;
              border-radius: 8px;
              padding: 6px 10px;
              font-size: 14px;
            }

            QDateTimeEdit::up-button {
                width: 18px;
                border-left: 1px solid #c0c0c0;
                background: #f5f5f5;
                image: url(../resources/up-arrow.png);
            }

            QDateTimeEdit::down-button {
                width: 18px;
                border-left: 1px solid #c0c0c0;
                background: #f5f5f5;
                image: url(../resources/down-arrow.png);
            }

            QDateTimeEdit::up-button:hover,
            QDateTimeEdit::down-button:hover {
                background: #e5e5e5;
            }
          )");

          return input;
        }
    }

  if (tn == "label")
    {
      auto *label = new QLabel (QString::fromStdString (n->get_text ()));

      label->setAttribute (Qt::WA_TranslucentBackground);

      label->setStyleSheet (R"(
          font-size: 16px;
        )");

      label->setAlignment (Qt::AlignLeft | Qt::AlignTop);
      label->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Fixed);

      return label;
    }

  if (tn == "div")
    {
      LOG ("saw 'div' tag\n");

      std::string attr_style = "", attr_width = "", attr_height = "",
                  attr_border = "", attr_display = "block";

      std::string fd = "row";
      std::string attr_justify_content = "flex-start",
                  attr_align_items = "stretch";
      std::string attr_gap = "", attr_grid_cols = "";

      try
        {
          attr_style = n->nd.get_attr ("style");
          LOG ("div has a 'style' attribute\n");
        }
      catch (const std::exception &e)
        {
          /* do nothing */
        }

      if (attr_style.size ())
        {
          auroral::CSSParser ap
              = auroral::CSSParser::parse_inline (attr_style);

          if (ap.nodes->has_key ("width"))
            attr_width = _str_trim (ap.nodes->get_ifstr ("width"));

          if (ap.nodes->has_key ("height"))
            attr_height = _str_trim (ap.nodes->get_ifstr ("height"));

          if (ap.nodes->has_key ("border"))
            attr_border = _str_trim (ap.nodes->get_ifstr ("border"));

          if (ap.nodes->has_key ("display"))
            attr_display = _str_trim (ap.nodes->get_ifstr ("display"));

          if (ap.nodes->has_key ("flex-direction"))
            fd = _str_trim (ap.nodes->get_ifstr ("flex-direction"));

          if (ap.nodes->has_key ("justify-content"))
            attr_justify_content
                = _str_trim (ap.nodes->get_ifstr ("justify-content"));

          if (ap.nodes->has_key ("align-items"))
            attr_align_items = _str_trim (ap.nodes->get_ifstr ("align-items"));

          if (ap.nodes->has_key ("gap"))
            attr_gap = _str_trim (ap.nodes->get_ifstr ("gap"));

          if (ap.nodes->has_key ("grid-template-columns"))
            attr_grid_cols
                = _str_trim (ap.nodes->get_ifstr ("grid-template-columns"));
        }

      LOG ("div display = '%s'\n", attr_display.data ());

      if (attr_display == "none")
        return nullptr;

      auto *container = new QWidget ();
      container->setAttribute (Qt::WA_TranslucentBackground);

      if (attr_border.size ())
        {
          unsigned long long rcc = RAND_COUNT++;
          std::string st_rcc
              = std::string{ "__rsb_Div_Container_" } + std::to_string (rcc);

          container->setObjectName (st_rcc);

          container->setStyleSheet (
              QString (("#" + st_rcc + " { border: %1; }").data ())
                  .arg (QString::fromStdString (attr_border)));
        }

      const bool is_inline
          = (attr_display == "inline" || attr_display == "inline-block"
             || attr_display == "inline-flex"
             || attr_display == "inline-grid");

      container->setSizePolicy (
          is_inline ? QSizePolicy::Preferred : QSizePolicy::Expanding,
          attr_height.size () ? QSizePolicy::Fixed : QSizePolicy::Preferred);

      apply_box_dimensions (container, attr_width, attr_height);

      const int gap_px
          = attr_gap.size ()
                ? auroral::css_scale_unit::parse_type (attr_gap).value
                : -1;

      if (attr_display == "grid" || attr_display == "inline-grid")
        {
          int cols = _count_grid_tracks (attr_grid_cols);
          LOG ("grid with %d column(s)\n", cols);

          auto *grid = new QGridLayout (container);
          grid->setContentsMargins (0, 0, 0, 0);
          grid->setSpacing (gap_px >= 0 ? gap_px : 0);

          int idx = 0;
          for (auto &i : n->children)
            {
              QWidget *iw = build_widget (i);
              if (!iw)
                continue;

              grid->addWidget (iw, idx / cols, idx % cols);
              idx++;
            }

          return container;
        }

      if (attr_display == "flex" || attr_display == "inline-flex")
        {
          LOG ("flex direction = '%s'\n", fd.data ());

          QBoxLayout *layout
              = (fd == "column")
                    ? static_cast<QBoxLayout *> (new QVBoxLayout (container))
                    : static_cast<QBoxLayout *> (new QHBoxLayout (container));

          layout->setContentsMargins (0, 0, 0, 0);
          if (gap_px >= 0)
            layout->setSpacing (gap_px);

          Qt::Alignment item_align = {};
          if (fd == "column")
            {
              if (attr_align_items == "center")
                item_align = Qt::AlignHCenter;
              else if (attr_align_items == "flex-end")
                item_align = Qt::AlignRight;
              else if (attr_align_items == "flex-start")
                item_align = Qt::AlignLeft;
            }
          else
            {
              if (attr_align_items == "center")
                item_align = Qt::AlignVCenter;
              else if (attr_align_items == "flex-end")
                item_align = Qt::AlignBottom;
              else if (attr_align_items == "flex-start")
                item_align = Qt::AlignTop;
            }

          /* justify-content -> main-axis distribution via stretch spacers. */
          const std::string &jc = attr_justify_content;

          int lead_stretch = (jc == "center" || jc == "flex-end"
                              || jc == "space-around" || jc == "space-evenly")
                                 ? 1
                                 : 0;
          int trail_stretch = (jc == "center" || jc == "flex-start"
                               || jc == "space-around" || jc == "space-evenly")
                                  ? 1
                                  : 0;
          int bet_stretch = (jc == "space-between" || jc == "space-evenly")
                                ? 1
                                : (jc == "space-around" ? 2 : 0);

          populate_layout (layout, n, item_align, lead_stretch, trail_stretch,
                           bet_stretch);

          return container;
        }

      /* Default display:block behavior. */
      QBoxLayout *layout
          = is_inline
                ? static_cast<QBoxLayout *> (new QHBoxLayout (container))
                : static_cast<QBoxLayout *> (new QVBoxLayout (container));

      layout->setContentsMargins (0, 0, 0, 0);
      if (gap_px >= 0)
        layout->setSpacing (gap_px);

      Qt::Alignment item_align = is_inline ? Qt::AlignVCenter : Qt::AlignLeft;

      populate_layout (layout, n, item_align, 0, 1, 0);

      return container;
    }

  if (tn == "button")
    {
      auto *btn = new QPushButton ();
      btn->setText (QString::fromStdString (n->get_text ()));
      btn->setCursor (Qt::PointingHandCursor);

      std::string attr_style = R"(
        background: #d1caca;
        border-radius: 4px;
        color: black;
      )";

      try
        {
          attr_style += n->nd.get_attr ("style");
        }
      catch (const std::exception &e)
        {
        }

      btn->setStyleSheet (QString::fromStdString (attr_style));

      return btn;
    }

  return nullptr;
}

void
v_browser::populate_layout (QBoxLayout *layout, equinox::Node *n,
                            Qt::Alignment item_align, int lead_stretch,
                            int trail_stretch, int between_stretch)
{
  if (lead_stretch)
    layout->addStretch (lead_stretch);

  int slot_count = 0;
  auto begin_slot = [&] ()
    {
      if (slot_count > 0 && between_stretch)
        layout->addStretch (between_stretch);
      slot_count++;
    };

  QWidget *inline_container = nullptr;
  QHBoxLayout *inline_row = nullptr;
  for (auto &i : n->children)
    {
      QWidget *iw = build_widget (i);

      if (i->nd.name == "br")
        {
          if (inline_row)
            inline_row->addStretch ();

          inline_row = nullptr;
          inline_container = nullptr;
        }

      if (!iw)
        continue;

      if (i->nd.name == "img" || i->nd.name == "input" || i->nd.name == "label"
          || i->nd.name == "button")
        {
          if (!inline_row)
            {
              begin_slot ();

              inline_container = new QWidget ();
              inline_container->setSizePolicy (QSizePolicy::Preferred,
                                               QSizePolicy::Fixed);

              inline_row = new QHBoxLayout (inline_container);
              inline_row->setAlignment (Qt::AlignLeft | Qt::AlignTop);
              inline_row->setSpacing (8);
              inline_row->setContentsMargins (0, 0, 0, 0);

              inline_container->setStyleSheet ("background: transparent;");

              layout->addWidget (inline_container, 0, item_align);
            }

          inline_row->addWidget (iw, 0, Qt::AlignBottom);
        }
      else
        {
          inline_row = nullptr;

          begin_slot ();
          layout->addWidget (iw, 0, item_align);
        }
    }

  if (trail_stretch)
    layout->addStretch (trail_stretch);
}

void
v_browser::apply_box_dimensions (QWidget *container, const std::string &width,
                                 const std::string &height)
{
  if (height.size ())
    {
      auroral::css_scale_unit cu
          = auroral::css_scale_unit::parse_type (height);

      switch (cu.type)
        {
        case auroral::AURORAL_CSS_SCALE_PX:
          container->setFixedHeight (cu.value);
          break;

        case auroral::AURORAL_CSS_SCALE_VIEWPORT_HEIGHT:
          container->setFixedHeight (cu.value * meta->height / 100);
          break;

        case auroral::AURORAL_CSS_SCALE_VIEWPORT_WIDTH:
          container->setFixedHeight (cu.value * meta->width / 100);
          break;

        default:
          break;
        }
    }

  if (width.size ())
    {
      auroral::css_scale_unit cu = auroral::css_scale_unit::parse_type (width);

      switch (cu.type)
        {
        case auroral::AURORAL_CSS_SCALE_PX:
          container->setFixedWidth (cu.value);
          break;

        case auroral::AURORAL_CSS_SCALE_VIEWPORT_HEIGHT:
          container->setFixedWidth (cu.value * meta->height / 100);
          break;

        case auroral::AURORAL_CSS_SCALE_VIEWPORT_WIDTH:
          container->setFixedWidth (cu.value * meta->width / 100);
          break;

        default:
          break;
        }
    }
}

void
v_browser::relayout_page ()
{
  rt_layout->removeWidget (page_content);

  page_content = build_widget (tree.get_tag_fromname ("body")[0]);
  rt_layout->addWidget (page_content, 15);
}

void
v_browser::resizeEvent (QResizeEvent *e)
{
  QWidget::resizeEvent (e);

  // relayout_page ();
  timer->start (3);
}
}; // namespace rs