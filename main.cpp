#include "header.hpp"
#include "w_browser.hpp"

int
main (int argc, char **argv)
{
  QApplication app (argc, argv);

  rs::w_browser w;
  w.show ();

  return app.exec ();
}