# Add files and directories to ship with the application
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
#TEMPLATE = app
#DEPLOYMENTFOLDERS = doc
TARGET = panicbutton

symbian:{
    TARGET.UID3 = 0xE623FE47

# Allow network access on Symbian
    TARGET.CAPABILITY += Location \
	NetworkServices \
	ReadUserData \
	WriteUserData

    TARGET.EPOCHEAPSIZE = 0x200000 0x4000000

# QtCreator takes care of the version for debian packages
# but not for Symbian
    VERSION = 0.5.0
}

SOURCES += main.cpp mainwindow.cpp \
    resizabletextqpushbutton.cpp \
    qcontactchooserdialog.cpp
HEADERS += mainwindow.h \
    resizabletextqpushbutton.h \
    qcontactchooserdialog.h
FORMS += mainwindow.ui \
    qcontactchooserdialog.ui

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the
# MOBILITY variable.
CONFIG += mobility
MOBILITY += contacts location messaging

# ady adyghe (west circassian)
# ar arabic
# ckb sorani (a kurdish language)
# fa farsi, persian
# kbd kabardian (east circassian)
# kmr kurmanji (a kurdish language)
# lki laki (a kurdish language)
# sdh southern kurdish
#
TRANSLATIONS += panicbutton_ady.ts \
    panicbutton_ar.ts \
    panicbutton_ckb.ts \
    panicbutton_fa.ts \
    panicbutton_kbd.ts \
    panicbutton_kmr.ts \
    panicbutton_lki.ts \
    panicbutton_sdh.ts


OTHER_FILES += \
    doc/panicbutton.html \
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


unix {
    maemo5 {
	# BUG: Should be able to set doc.path to
	# $${installPrefix}/share/doc/panicbutton
	# Alas, installPrefix isn't defined until
	# deployment.pri, which we can't load until
	# the very end.
	doc.path = /opt/panicbutton/share/doc
	target.path = /opt/panicbutton/bin
    } else:contains(MEEGO_EDITION,harmattan) {
	doc.path = /opt/panicbutton/share/doc
	target.path = /opt/panicbutton/bin
    } else {
    }

    doc.files += doc/panicbutton.html

    INSTALLS += doc
    INSTALLS += target
}

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

