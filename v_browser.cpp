#include "v_browser.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace rs
{
v_browser::v_browser (int id, QWidget *parent) : QWidget (parent), m_id (id)
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

  auto *page_content
      = new QLabel (QString ("Default page for tab %1").arg (m_id));

  page_content->setAlignment (Qt::AlignCenter);

  page_content->setStyleSheet (R"(
      background: white;
      color: black;
      font-size: 32px;
  )");

  rt_layout->addWidget (top_bar, 1);
  rt_layout->addWidget (page_content, 15);
}

};