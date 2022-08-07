
QT       -= core
QT       -= gui

TARGET = nrc_
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += object_parallel_to_source #https://wiki.qt.io/Undocumented_QMake; if your version doesn't support, rename osbs/unix/sinfo.c and bsocket.c

DEFINES += CMAKE_RELEASE NAPPGUI_BUILD
INCLUDEPATH += nlib ../core ../osbs ../sewer

TEMPLATE = app

SOURCES += \
    nrc/nrc.c \
    nlib/bnfparser.c \
    nlib/msgparser.c \
    nlib/nglob.c \
    nlib/nrclib.c \
    nlib/resgen.c \
    nlib/ssh.c

include(core.pri)
