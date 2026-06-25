#include "w_browser.hpp"

namespace rs
{
w_browser::w_browser ()
{
  resize (1200, 800);

  setWindowTitle (QString ("RSBrowser"));
  setStyleSheet ("background: white; color: black;");

  // QApplication::setFont (QFont ("Arial", 14));
  // QApplication::setFont (QFont (".AppleSystemUIFont", 14));
  // QApplication::setFont (QFont ("Times New Roman", 14));

  QFont font;

  font.setFamilies ({ /* "Inter", "SF Pro Text", */ ".AppleSystemUIFont",
                      "Helvetica Neue", "Arial" });

  font.setPointSize (14);

  QApplication::setFont (font);

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

  /* get new tab html */
  std::ifstream f ("../resources/counter.html");

  if (!f.is_open ())
    {
      /* TODO */
      std::cout << "new tab html not found\n";
      exit (EXIT_FAILURE);
    }

  std::stringstream buf;
  buf << f.rdbuf ();
  std::string fc = buf.str ();

  new_tab_html = equinox::HTMLParser (fc);
  new_tab_html.build_tree ();

  equinox::Node *nt = new_tab_html.get_tree ();

  // while (nt != nullptr)
  //   {
  //     std::cout << nt->nd.name << '\n';
  //     nt = nt->next;
  //   }

  f.close ();

  setCentralWidget (m_tabs);
  add_tab ();
}

void
w_browser::add_tab ()
{
  int id = m_tabs->count () + 1;

  if (vbm == nullptr)
    {
      vbm = new __v_browser_meta ();
      vbm->height = height ();
      vbm->width = width ();
    }

  v_browser *view = new v_browser (id, new_tab_html, vbm);

  std::vector<equinox::Node *> tv = new_tab_html.get_tag_fromname ("title");

  if (!tv.size ())
    m_tabs->addTab (view, QString ("Tab %1").arg (id));
  else
    m_tabs->addTab (view, QString (tv[0]->get_text ().data ()));
}

void
w_browser::resizeEvent (QResizeEvent *e)
{
  QWidget::resizeEvent (e);

  if (vbm == nullptr)
    {
      vbm->height = height ();
      vbm->width = width ();
    }
  else
    {
      vbm = new __v_browser_meta ();
      vbm->height = height ();
      vbm->width = width ();
    }
}
} // namespace rs
