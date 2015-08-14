#-------------------------------------------------
#
# Project created by QtCreator 2011-08-01T13:56:14
#
#-------------------------------------------------

TARGET = qcontactchooserdialog
TEMPLATE = lib

DEFINES += QCONTACTCHOOSERDIALOG_LIBRARY

SOURCES += qcontactchooserdialog.cpp

HEADERS += qcontactchooserdialog.h\
        qcontactchooserdialog_global.h

symbian {
#    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE03008DE
#
# This library requires no special capabilities
#
#    TARGET.CAPABILITY = ALL -TCB
    TARGET.CAPABILITY += Location \
        NetworkServices \
        ReadUserData \
        WriteUserData
#
# This library has no writable static data
#
#    TARGET.EPOCALLOWDLLDATA = 1

#
# This deploys the headers to the SDK's /epoch32/include directory.
# It works only for Symbian.
#
    BLD_INF_RULES.prj_exports += "qcontactchooserdialog.h"
    BLD_INF_RULES.prj_exports += "qcontactchooserdialog_global.h"

    addFiles.sources = qcontactchooserdialog.dll
    addFiles.path = /sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
#
# What's the unix equivalent of BLD_INF_RULES.prj_exports?
#
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

FORMS += \
    qcontactchooserdialog.ui
