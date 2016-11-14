#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T09:20:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CustomWindow
TEMPLATE = app


SOURCES += main.cpp\
    customwindow.cpp \
    cursorpage.cpp \
    screenbreakpage.cpp \
    bluelightreducerpage.cpp

HEADERS  += \
    customwindow.h \
    cursorpage.h \
    screenbreakpage.h \
    bluelightreducerpage.h

FORMS    += \
    customwindow.ui \
    cursorpage.ui \
    screenbreakpage.ui \
    bluelightreducerpage.ui

RESOURCES += \
    resources.qrc
