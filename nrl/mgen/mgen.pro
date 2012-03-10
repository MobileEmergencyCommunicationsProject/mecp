#-------------------------------------------------
#
# Project created by QtCreator 2011-09-09T18:07:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = mgen

TEMPLATE = app

INCLUDEPATH += ./protolib/include ./include

#
# Don't know if protolib for OS X is a library or a framework.
# Does it matter?
#
unix:!macx: LIBS += -lprotolib -lnetfilter_queue
#-lpcap

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

unix:!symbian {
    maemo5 {
	target.path = /opt/usr/bin

	QMAKE_LFLAGS += '-Wl,-rpath,/opt/usr/lib'

    } else {
	target.path = /usr/bin
    }

    DEFINES += _FILE_OFFSET_BITS=64 \
    _RAPR_JOURNAL \
    HAVE_ASSERT \
    HAVE_DIRFD \
    HAVE_GETLOGIN \
    HAVE_GPS \
    HAVE_IPV6 \
    HAVE_LOCKF \
    HAVE_PSELECT \
    HAVE_SCHED \
    LINUX \
    PROTO_DEBUG \
    UNIX

#$(NETSEC)

    INCLUDEPATH += ./protolib/include/unix

    INSTALLS += target

    LIBS += -L/opt/usr/lib

    SOURCES +=

}

SOURCES += \
    src/common/gpsPub.cpp \
    src/common/mgen.cpp \
    src/common/mgenApp.cpp \
    src/common/mgenAppSinkTransport.cpp \
    src/common/mgenEvent.cpp \
    src/common/mgenFlow.cpp \
    src/common/mgenMsg.cpp \
    src/common/mgenPattern.cpp \
    src/common/mgenPayload.cpp \
    src/common/mgenSequencer.cpp \
    src/common/mgenTransport.cpp

HEADERS += \
    include/mgenVersion.h \
    include/mgenTransport.h \
    include/mgenSequencer.h \
    include/mgenPayload.h \
    include/mgenPattern.h \
    include/mgenMsg.h \
    include/mgenGlobals.h \
    include/mgenFlow.h \
    include/mgenEvent.h \
    include/mgenApp.h \
    include/mgen.h \
    include/gpsPub.h


















