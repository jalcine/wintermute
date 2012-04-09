cmake_minimum_required(VERSION 2.8)

option(WITH_GUI_ADDON       "Build support for the GUI application?"            OFF)
option(WITH_NLUI_ADDON      "Build support for (experimental) NLUI interface?"  OFF)
option(WITH_NCURSES_ADDON   "Build support for nCurses interface?"              ON)

if (WITH_GUI_ADDON)
    add_subdirectory(gui-wintermute)
endif(WITH_GUI_ADDON)

if (WITH_NLUI_ADDON)
    add_subdirectory(nlui-wintermute)
endif(WITH_NLUI_ADDON)

if (WITH_NCURSES_ADDON)
    add_subdirectory(ncurses-wintermute)
endif(WITH_NCURSES_ADDON)