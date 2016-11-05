#-------------------------------------------------
#
# Project created by QtCreator 2016-01-18T09:04:28
#
#-------------------------------------------------

QT       += core gui network xml quick

TEMPLATE = lib
DEFINES += UTILS_LIBRARY

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
    TARGET = utilsd
} else {
    TARGET = utils
}

HEADERS += \
    cxmlnode.h \
    utils.h \
    utils_global.h

SOURCES += \
    cxmlnode.cpp \
    utils.cpp
