#-------------------------------------------------
#
# Project created by QtCreator 2011-09-05T13:34:13
#
#-------------------------------------------------

QT       -= core gui

TARGET = nrlolsrd
TEMPLATE = app

DEFINES += NRLOLSR_LIBRARY

INCLUDEPATH += ./common ./protolib/include

#
# Don't know what libs to use for OS X and Symbian
#
unix:!macx:!symbian: LIBS += -lpthread

#
# Don't know if protolib for OS X is a library or a framework.
# Does it matter?
#
unix:!macx: LIBS += -lprotolib

SOURCES += \
    ./common/olsr_packet_types.cpp \
    ./common/nrlolsrApp.cpp \
    ./common/nrlolsr.cpp \
    ./common/nbr_queue.cpp \
    ./common/mprselPub.cpp

HEADERS += \
    ./common/olsr_packet_types.h \
    ./common/nrlolsrgui.h \
    ./common/nrlolsrApp.h \
    ./common/nrlolsr.h \
    ./common/nbr_queue.h \
    ./common/mprselPub.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3432B2A
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = nrlolsr.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin

        QMAKE_LFLAGS += '-Wl,-rpath,/opt/usr/lib'

    } else {
        target.path = /usr/bin
    }

    DEFINES += HAVE_ASSERT HAVE_DIRFD \
    HAVE_GETLOGIN HAVE_IPV6 \
    HAVE_LOCKF HAVE_OLD_SIGNALHANDLER \
    LINUX PROTO_DEBUG SMF_SUPPORT UNIX

#$(NETSEC)

    INCLUDEPATH += ./protolib/include/unix

    INSTALLS += target

    LIBS += -L/opt/usr/lib

    SOURCES += protolib/src/linux/linuxCap.cpp

# WX-based gui
#     ./common/nrlolsrgui.cpp \
}

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog


