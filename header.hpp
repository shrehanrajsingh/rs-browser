#if !defined(HEADER_H)
#define HEADER_H

#include <cassert>
#include <cctype>
#include <complex>
#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <QApplication>

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStyleFactory>
#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtSql>
#include <QtWidgets>
#include <QtXml>

// HTML Engine (Equinox)
#include "equinox/equinox.hpp"

// CSS Engine (Auroral)
#include "auroral/auroral.hpp"

#ifndef here
#define here std::printf ("%s (%s): %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

#ifndef LOG
#define LOG(...)                                                              \
  {                                                                           \
    std::printf ("[LOG] (%s:%d) ", __FUNCTION__, __LINE__);                   \
    std::printf (__VA_ARGS__);                                                \
  }
#endif

#endif // HEADER_H
