#-------------------------------------------------
#
# Project created by QtCreator 2013-01-29T19:21:34
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = torrify
TEMPLATE = app


SOURCES +=\
    src/torinstancelistmodel.cpp \
    src/torinstancemanager.cpp \
    src/torcontrol.cpp \
    src/torinstance.cpp \
    src/torsettings.cpp \
    src/torlocationfinder.cpp \
    src/helpdialog.cpp \
    src/main.cpp \
    src/torrifywindow.cpp

HEADERS  += \
    src/torinstancelistmodel.h \
    src/torinstancemanager.h \
    src/torcontrol.h \
    src/torlauncher.h \
    src/TorInterfaces.h \
    src/torinstance.h \
    src/torsettings.h \
    src/torlocationfinder.h \
    src/helpdialog.h \
    src/torrifywindow.h \
    src/version.h

FORMS    += \
    src/helpdialog.ui \
    src/torrifywindow.ui

RESOURCES += \
    torrify.qrc

RC_FILE = torrify.rc

OTHER_FILES += \
    torrify.rc \
    license.html
