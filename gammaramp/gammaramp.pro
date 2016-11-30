#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T16:29:57
#
#-------------------------------------------------

QT       += core

TEMPLATE = lib
DEFINES += GAMMARAMP_LIBRARY

unix {
    DESTDIR = ../bin
    MOC_DIR = ../moc
    OBJECTS_DIR = ../obj
}

win32 {
    DESTDIR = ..\\bin
    MOC_DIR = ..\\moc
    OBJECTS_DIR = ..\\obj
    LIBS += -luser32
}

macx {
    LIBS += -framework Cocoa -framework CoreGraphics
}

QMAKE_CLEAN *= $$DESTDIR\\*$$TARGET*
QMAKE_CLEAN *= $$MOC_DIR\\*$$TARGET*
QMAKE_CLEAN *= $$OBJECTS_DIR\\*$$TARGET*

CONFIG(debug, debug|release) {
    TARGET = gammarampd
} else {
    TARGET = gammaramp
}

HEADERS += \
    imonitor.h \
    monitor.h \
    gammaramp_global.h

win32 {
    HEADERS += windowsmonitor.h
}
macx {
    HEADERS += macmonitor.h
}

SOURCES += \
    monitor.cpp

win32 {
    SOURCES += windowsmonitor.cpp
}
macx {
    SOURCES += macmonitor.cpp
}
