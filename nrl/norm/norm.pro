#-------------------------------------------------
#
# Project created by QtCreator 2011-09-07T14:22:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = norm
TEMPLATE = lib

DEFINES += NORM_LIBRARY

INCLUDEPATH += ./include ./protolib/include

#
# Don't know what libs to use for OS X and Symbian
#
unix:!macx:!symbian: LIBS += -lpthread

# -lnetfilter_queue

#
# Don't know if protolib for OS X is a library or a framework.
# Does it matter?
#
unix:!macx: LIBS += -lprotolib

SOURCES += src/common/normSession.cpp \
    src/common/normSegment.cpp \
    src/common/normObject.cpp \
    src/common/normNode.cpp \
    src/common/normMessage.cpp \
    src/common/normFile.cpp \
    src/common/normEncoderRS16.cpp \
    src/common/normEncoderRS8.cpp \
    src/common/normEncoderMDP.cpp \
    src/common/normEncoder.cpp \
    src/common/normApi.cpp \
    src/common/galois.cpp

HEADERS += include/normVersion.h \
    include/normSession.h \
    include/normSegment.h \
    include/normObject.h \
    include/normNode.h \
    include/normMessage.h \
    include/normFile.h \
    include/normEncoderRS16.h \
    include/normEncoderRS8.h \
    include/normEncoderMDP.h \
    include/normEncoder.h \
    include/normApi.h \
    include/galois.h

LIBS += -lprotolib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEFBCC88F
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = norm.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }

    DEFINES += _FILE_OFFSET_BITS=64 ECN_SUPPORT \
        HAVE_ASSERT HAVE_DIRFD \
        HAVE_GETLOGIN HAVE_IPV6 \
        HAVE_LOCKF HAVE_OLD_SIGNALHANDLER \
        LINUX PROTO_DEBUG UNIX

    INSTALLS += target

}

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    src/common/rules.txt











