#include "v_browser.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace rs
{
v_browser::v_browser (int id, equinox::HTMLParser &_Tree, QWidget *parent)
    : QWidget (parent), m_id (id), tree{ _Tree }
{
  auto *rt_layout = new QVBoxLayout (this); /* root layotu */

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

  auto *page_content = build_widget (tree.get_tag_fromname ("body")[0]);

  // page_content->setAlignment (Qt::AlignCenter);

  page_content->setStyleSheet (R"(
      background: white;
      color: black;
      font-size: 32px;
  )");

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
      auto *layout = new QVBoxLayout (container);

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
              || i->nd.name == "label")
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

      try
        {
          inp_type = n->nd.get_attr ("type");
        }
      catch (const std::exception &e)
        {
          inp_type = "text"; /* fallback */
        }

      if (inp_type[0] == '\"' || inp_type[0] == '\'')
        inp_type = inp_type.substr (1, inp_type.size () - 2);

      if (inp_type == "text")
        {
          auto *input = new QLineEdit ();

          input->setStyle (QStyleFactory::create ("Fusion"));
          input->setStyleSheet (R"(
              QLineEdit {
                background-color: white;
                color: #000;
                font-size: 14px;
              }
            )");

          input->setFixedWidth (250);

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

      label->setStyleSheet (R"(
          font-size: 16px;
        )");

      label->setAlignment (Qt::AlignLeft | Qt::AlignTop);
      label->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Fixed);

      return label;
    }

  return nullptr;
}

};