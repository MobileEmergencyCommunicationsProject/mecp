#-------------------------------------------------
#
# Project created by QtCreator 2011-09-09T18:07:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = nrlsmf

TEMPLATE = app

INCLUDEPATH += ./protolib/include /opt/include

#
# Don't know if protolib for OS X is a library or a framework.
# Does it matter?
#
unix:!macx: LIBS += -lprotolib -lnetfilter_queue -lnfnetlink
#-lpcap

SOURCES += \
    common/smfHashSHA1.cpp \
    common/smfHashMD5.cpp \
    common/smfHash.cpp \
    common/smfDpd.cpp \
    common/smf.cpp \
    common/nrlsmf.cpp

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

HEADERS += \
    common/smfVersion.h \
    common/smfHashSHA1.h \
    common/smfHashMD5.h \
    common/smfHash.h \
    common/smfDpd.h \
    common/smf.h

unix:!symbian {
    maemo5 {
	target.path = /opt/usr/bin

	QMAKE_LFLAGS += '-Wl,-rpath,/opt/usr/lib -Wl,-rpath,/opt/lib'

    } else {
	target.path = /usr/bin
    }

    DEFINES += _FILE_OFFSET_BITS=64 \
    HAVE_ASSERT HAVE_DIRFD \
    HAVE_GETLOGIN HAVE_IPV6 \
    HAVE_LOCKF HAVE_OLD_SIGNALHANDLER \
    HAVE_SCHED \
    LINUX \
    MAEMO \
    MNE_SUPPORT NO_SCM_RIGHTS PROTO_DEBUG SMF_SUPPORT UNIX

#$(NETSEC)

    INCLUDEPATH += ./protolib/include/unix

    INSTALLS += target

    LIBS += -L/opt/usr/lib -L/opt/lib

    SOURCES += protolib/src/linux/linuxCap.cpp \
    protolib/src/linux/linuxDetour.cpp

}

