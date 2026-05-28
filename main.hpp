#if !defined(RSBROWSER_MAIN_H)
#define RSBROWSER_MAIN_H

#include "header.hpp"
#include "w_browser.hpp"

/**
 * \mainpage RSBrowser
 *
 * A Qt6 desktop prototype that renders a small subset of HTML into native
 * widgets. The UI provides a tabbed window with a simple navigation bar and a
 * default "new tab" page loaded from resources/home.html.
 *
 * \image html docs/images/view.png "RSBrowser UI" width=1000px
 *
 * \section intro_sec Introduction
 * RSBrowser is a learning-oriented browser experiment. It focuses on a simple
 * HTML parsing pipeline and a lightweight widget renderer rather than full web
 * standards compliance.
 *
 * \section architecture_sec Architecture
 * - Equinox: HTML engine/parser used by the current build.
 * - Auroral: CSS engine (planned/placeholder).
 * - Solstice: JavaScript engine (planned/placeholder).
 * - UI: Qt widgets in w_browser/v_browser build the view tree from parsed
 * nodes.
 *
 * \section setup_sec Setup
 * - Install Qt6 with Widgets, Core, Gui, Network, Sql, and Xml components.
 * - Point CMake to your Qt install by updating CMAKE_PREFIX_PATH in
 *   CMakeLists.txt or passing -DCMAKE_PREFIX_PATH=/path/to/Qt at configure
 * time.
 * - Use a C++17-capable compiler and CMake 3.10 or newer.
 *
 * \section build_sec Build
 * \code
 * cmake -S . -B build
 * cmake --build build
 * \endcode
 *
 * \section usage_sec Usage
 * Run the executable from the build directory so relative resources resolve:
 * \code
 * cd build
 * ./rsbrowser
 * \endcode
 * The default tab content is parsed from ../resources/home.html and rendered
 * into widgets.
 *
 * \section status_sec Status
 * The renderer currently supports a small tag set (headings, paragraphs,
 * images, inputs, labels, and line breaks). CSS and JavaScript engines are not
 * yet wired into the application.
 */

#endif // RSBROWSER_MAIN_H
