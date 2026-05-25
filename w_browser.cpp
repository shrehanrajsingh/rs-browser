#include "w_browser.hpp"

namespace rs
{
w_browser::w_browser ()
{
  resize (1200, 800);

  setWindowTitle (QString ("RSBrowser"));
  setStyleSheet ("background: white; color: black;");

  m_tabs = new QTabWidget;

  m_tabs->setDocumentMode (true);
  m_tabs->setMovable (true);
  m_tabs->setTabsClosable (true);
  //   m_tabs->setTabPosition (QTabWidget::South);

  /* new tab button */
  QPushButton *nt_btn = new QPushButton ("+");
  nt_btn->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);
  //   nt_btn->resize (28, 28);

  m_tabs->setStyleSheet (R"(
    QPushButton {
        margin: 0;
        padding: 5px 15px;
        border: none;
        color: white;
        font-size: 22px;
        background: #222;
    }
    )");

  m_tabs->setCornerWidget (nt_btn, Qt::TopLeftCorner);

  connect (m_tabs, &QTabWidget::tabCloseRequested, this,
           [this] (int index)
             {
               if (m_tabs->count () < 2)
                 {
                   /* dont delete if only 1 tab */
                   return;
                 }

               QWidget *tab = m_tabs->widget (index);

               m_tabs->removeTab (index);

               delete tab;
             });

  connect (nt_btn, &QPushButton::clicked, this,
           [this] ()
             {
               add_tab ();
               m_tabs->setCurrentIndex (m_tabs->count () - 1);
             });

  setCentralWidget (m_tabs);
  add_tab ();
}

void
w_browser::add_tab ()
{
  int id = m_tabs->count () + 1;

  v_browser *view = new v_browser (id);

  m_tabs->addTab (view, QString ("Tab %1").arg (id));
}
} // namespace rs
