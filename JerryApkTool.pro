#-------------------------------------------------
#
# Project created by QtCreator 2014-12-28T11:23:23
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JerryApkTool
TEMPLATE = app

SOURCES += main.cpp\
        dialog.cpp \
    toolwindow.cpp \
    commandlinethread.cpp \
    parsemanifest.cpp \
    commandprocess.cpp

HEADERS  += dialog.h \
    toolwindow.h \
    commandlinethread.h \
    parsemanifest.h \
    commandprocess.h

FORMS    += dialog.ui \
    toolwindow.ui
