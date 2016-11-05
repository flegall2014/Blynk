#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T16:29:57
#
#-------------------------------------------------

QT       += gui widgets network

TEMPLATE = lib
DEFINES += SINGLEAPPLICATION_LIBRARY

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
    TARGET = singleapplicationd
} else {
    TARGET = singleapplication
}

SOURCES += \
    qtlocalpeer.cpp \
    qtlockedfile.cpp \
    qtlockedfile_unix.cpp \
    qtlockedfile_win.cpp \
    qtsingleapplication.cpp \
    qtsinglecoreapplication.cpp

HEADERS +=\
        singleapplication_global.h \
    qtlocalpeer.h \
    qtlockedfile.h \
    qtsingleapplication.h \
    qtsinglecoreapplication.h

