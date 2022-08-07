INCLUDEPATH += ../osbs ../sewer ../core
DEFINES += NAPPGUI_LIBRARY CMAKE_RELEASE NAPPGUI_BUILD_DIR NAPPGUI_SOURCE_DIR NAPPGUI_BUILD

SOURCES += \
    ../core/core.cpp \
    ../core/event.cpp \
    ../osbs/osbs.cpp \
    ../sewer/bmath.cpp \
    ../sewer/sewer.cpp \
    ../core/array.c \
    ../core/bhash.c \
    ../core/buffer.c \
    ../core/clock.c \
    ../core/date.c \
    ../core/dbind.c \
    ../core/heap.c \
    ../core/hfile.c \
    ../core/keybuf.c \
    ../core/lex.c \
    ../core/nfa.c \
    ../core/obj.c \
    ../core/rbtree.c \
    ../core/regex.c \
    ../core/respack.c \
    ../core/stream.c \
    ../core/strings.c \
    ../core/tfilter.c \
    ../osbs/log.c \
    ../osbs/bsocket.c \
    ../sewer/blib.c \
    ../sewer/bmem.c \
    ../sewer/cassert.c \
    ../sewer/ptr.c \
    ../sewer/qsort.c \
    ../sewer/types.c \
    ../sewer/unicode.c

linux {
INCLUDEPATH += /usr/include/glib-2.0 /usr/lib/i386-linux-gnu/glib-2.0/include 
LIBS += -L/usr/lib/i386-linux-gnu -lglib-2.0
SOURCES += \
    ../osbs/linux/sinfo.c \
    ../osbs/unix/bfile.c \
    ../osbs/unix/bmutex.c \
    ../osbs/unix/bproc.c \
    ../osbs/unix/bthread.c \
    ../osbs/unix/btime.c \
    ../sewer/unix/bstdimp.c \
    ../sewer/unix/bmem_unix.c \
    ../osbs/unix/bsocket.c \
    ../osbs/unix/sinfo.c
}

win32 {
DEFINES += _WINDOWS
SOURCES += \
    ../osbs/win/bfile.c \
    ../osbs/win/bmutex.c \
    ../osbs/win/bproc.c \
    ../osbs/win/bsocket.c \
    ../osbs/win/bthread.c \
    ../osbs/win/btime.c \
    ../osbs/win/sinfo.c \
    ../sewer/win/bmem_win.c \
    ../sewer/win/bstdimp.c
    
LIBS += -lGdi32 -lUser32 -lShell32 -lComdlg32
}

macx {
SOURCES += \
    ../osbs/unix/bfile.c \
    ../osbs/unix/bmutex.c \
    ../osbs/unix/bproc.c \
    ../osbs/unix/bthread.c \
    ../osbs/unix/btime.c \
    ../osbs/unix/bsocket.c \
        ../osbs/unix/sinfo.c \
        ../sewer/unix/bmem_unix.c \
        ../sewer/unix/bstdimp.c \
        ../osbs/osx/sinfo.m

LIBS += -framework CoreFoundation -framework AppKit
}


