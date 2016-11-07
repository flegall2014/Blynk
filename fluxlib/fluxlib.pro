#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T16:29:57
#
#-------------------------------------------------

QT       += core

TEMPLATE = lib
DEFINES += FLUXLIB_LIBRARY

unix {
    DESTDIR = ../bin
    MOC_DIR = ../moc
    OBJECTS_DIR = ../obj
}

win32 {
    DESTDIR = ..\\bin
    MOC_DIR = ..\\moc
    OBJECTS_DIR = ..\\obj
}

QMAKE_CLEAN *= $$DESTDIR\\*$$TARGET*
QMAKE_CLEAN *= $$MOC_DIR\\*$$TARGET*
QMAKE_CLEAN *= $$OBJECTS_DIR\\*$$TARGET*

CONFIG(debug, debug|release) {
    TARGET = fluxlibd
} else {
    TARGET = fluxlib
}

HEADERS += \
    fluxlib.h \
    fluxlib_global.h

SOURCES += \
    fluxlib.cpp




