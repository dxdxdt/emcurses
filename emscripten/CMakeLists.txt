CMAKE_MINIMUM_REQUIRED(VERSION 3.1...3.5)
PROJECT(emcurses C)

include(GNUInstallDirs)

SET(EMCURSES_MAJOR_VERSION 3)
SET(EMCURSES_MINOR_VERSION 4)
SET(VERSION "${EMCURSES_MAJOR_VERSION}.${EMCURSES_MINOR_VERSION}")
SET(EMCURSES_VERSION "${EMCURSES_MAJOR_VERSION}.${EMCURSES_MINOR_VERSION}")

IF (NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    MESSAGE("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
	MESSAGE(FATAL_ERROR "Target is not Emscripten!")
ENDIF()

ADD_COMPILE_OPTIONS("-I${PROJECT_SOURCE_DIR}/..")

SET(EMCURSES_O "o")
SET(PDCURSES_SRCDIR "${PROJECT_SOURCE_DIR}/../pdcurses")
SET(EMCURSES_LIB_MODULES
    # LIBOBJS
    "${PDCURSES_SRCDIR}/addch.c"
    "${PDCURSES_SRCDIR}/addchstr.c"
    "${PDCURSES_SRCDIR}/addstr.c"
    "${PDCURSES_SRCDIR}/attr.c"
    "${PDCURSES_SRCDIR}/beep.c"
    "${PDCURSES_SRCDIR}/bkgd.c"
    "${PDCURSES_SRCDIR}/border.c"
    "${PDCURSES_SRCDIR}/clear.c"
    "${PDCURSES_SRCDIR}/color.c"
    "${PDCURSES_SRCDIR}/delch.c"
    "${PDCURSES_SRCDIR}/deleteln.c"
    "${PDCURSES_SRCDIR}/deprec.c"
    "${PDCURSES_SRCDIR}/getch.c"
    "${PDCURSES_SRCDIR}/getstr.c"
    "${PDCURSES_SRCDIR}/getyx.c"
    "${PDCURSES_SRCDIR}/inch.c"
    "${PDCURSES_SRCDIR}/inchstr.c"
    "${PDCURSES_SRCDIR}/initscr.c"
    "${PDCURSES_SRCDIR}/inopts.c"
    "${PDCURSES_SRCDIR}/insch.c"
    "${PDCURSES_SRCDIR}/insstr.c"
    "${PDCURSES_SRCDIR}/instr.c"
    "${PDCURSES_SRCDIR}/kernel.c"
    "${PDCURSES_SRCDIR}/keyname.c"
    "${PDCURSES_SRCDIR}/mouse.c"
    "${PDCURSES_SRCDIR}/move.c"
    "${PDCURSES_SRCDIR}/outopts.c"
    "${PDCURSES_SRCDIR}/overlay.c"
    "${PDCURSES_SRCDIR}/pad.c"
    "${PDCURSES_SRCDIR}/panel.c"
    "${PDCURSES_SRCDIR}/printw.c"
    "${PDCURSES_SRCDIR}/refresh.c"
    "${PDCURSES_SRCDIR}/scanw.c"
    "${PDCURSES_SRCDIR}/scr_dump.c"
    "${PDCURSES_SRCDIR}/scroll.c"
    "${PDCURSES_SRCDIR}/slk.c"
    "${PDCURSES_SRCDIR}/termattr.c"
    "${PDCURSES_SRCDIR}/terminfo.c"
    "${PDCURSES_SRCDIR}/touch.c"
    "${PDCURSES_SRCDIR}/util.c"
    "${PDCURSES_SRCDIR}/window.c"
    "${PDCURSES_SRCDIR}/debug.c"
    # PDCOBJS
    pdcclip.c
    pdcdisp.c
    pdcgetsc.c
    pdckbd.c
    pdcscrn.c
    pdcsetsc.c
    pdcutil.c
)

SET(EMCURSES_LIBS)
ADD_LIBRARY(emcurses ${EMCURSES_LIB_MODULES})
LIST (APPEND EMCURSES_LIBS "emcurses")
SET(prefix ${CMAKE_INSTALL_PREFIX})
SET(exec_prefix "${prefix}")
SET(includedir "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
SET(libdir "${CMAKE_INSTALL_FULL_LIBDIR}")
CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/emcurses.pc.in"
    "${PROJECT_BINARY_DIR}/emcurses.pc"
    @ONLY)

IF (TRUE)
    SET(EMCURSES_DEMO_LDFLAGS --pre-js ${PROJECT_SOURCE_DIR}/termlib.js)

    ADD_EXECUTABLE(firework firework.c)
    TARGET_LINK_OPTIONS(firework PRIVATE ${EMCURSES_DEMO_LDFLAGS})
    TARGET_LINK_LIBRARIES(firework emcurses)
    CONFIGURE_FILE(
        "${PROJECT_SOURCE_DIR}/firework.html"
        "${PROJECT_BINARY_DIR}/firework.html"
        COPYONLY)
    ADD_EXECUTABLE(rain rain.c)
    TARGET_LINK_OPTIONS(rain PRIVATE ${EMCURSES_DEMO_LDFLAGS})
    TARGET_LINK_LIBRARIES(rain emcurses)
    CONFIGURE_FILE(
        "${PROJECT_SOURCE_DIR}/rain.html"
        "${PROJECT_BINARY_DIR}/rain.html"
        COPYONLY)
ENDIF()

INSTALL(
    TARGETS ${EMCURSES_LIBS}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
INSTALL(
    FILES
        "${PROJECT_SOURCE_DIR}/../curses.h"
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
)
INSTALL(
    FILES
        "${PROJECT_SOURCE_DIR}/termlib.js"
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/emcurses
)
INSTALL(
    FILES
        "${PROJECT_BINARY_DIR}/emcurses.pc"
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
