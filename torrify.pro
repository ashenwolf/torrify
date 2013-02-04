#-------------------------------------------------
#
# Project created by QtCreator 2013-01-29T19:21:34
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = torrify
TEMPLATE = app


SOURCES += main.cpp\
        torrifywindow.cpp \
    src/torinstancelistmodel.cpp \
    src/torinstancemanager.cpp \
    src/torcontrol.cpp \
    src/torlauncher.cpp

HEADERS  += torrifywindow.h \
    src/torinstancelistmodel.h \
    src/torinstancemanager.h \
    src/torcontrol.h \
    src/torlauncher.h \
    src/TorInterfaces.h

FORMS    += torrifywindow.ui

RESOURCES += \
    torrify.qrc
