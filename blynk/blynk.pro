#-------------------------------------------------
#
# Project created by QtCreator 2016-10-23T09:16:18
#
#-------------------------------------------------

QT       += core gui xml widgets

TEMPLATE = app

INCLUDEPATH += $$PWD/../utils ../singleapplication ../gammaramp ../blrlib

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../bin/ -lutilsd -lsingleapplicationd -lgammarampd -lblrlibd
    TARGET = blynkd
} else {
    LIBS += -L$$PWD/../bin/ -lutils -lsingleapplication -lgammaramp -lblrlib
    TARGET = blynk
}

unix {
    DESTDIR = ../bin
    MOC_DIR = ./moc
    OBJECTS_DIR = ./obj
}

win32 {
    DESTDIR = ..\\bin
    MOC_DIR = .\\moc
    OBJECTS_DIR = .\\obj
}

unix {
    QMAKE_CLEAN *= $$DESTDIR/*$$TARGET*
    QMAKE_CLEAN *= $$MOC_DIR/*moc_*
    QMAKE_CLEAN *= $$OBJECTS_DIR/*.o*
}

win32 {
    QMAKE_CLEAN *= $$DESTDIR\\*$$TARGET*
    QMAKE_CLEAN *= $$MOC_DIR\\*moc_*
    QMAKE_CLEAN *= $$OBJECTS_DIR\\*.o*
}

HEADERS += \
    controller.h \
    blynk.h \
    iservice.h \
    imagewidget.h \
    parameters.h \
    utils.h \
    bluelightreducerwidget.h \
    customslider.h \
    dimmerwidget.h \
    bluelightreducerpage.h \
    customwindow.h \
    screenbreakpage.h \
    cursorpage.h \
    settingswindow.h \
    aboutwindow.h \
    animatedimage.h \
    blynkwindow.h \
    bluelightreducerslider.h

SOURCES += \
    main.cpp \
    controller.cpp \
    blynk.cpp \
    imagewidget.cpp \
    parameters.cpp \
    utils.cpp \
    bluelightreducerwidget.cpp \
    customslider.cpp \
    dimmerwidget.cpp \
    bluelightreducerpage.cpp \
    customwindow.cpp \
    screenbreakpage.cpp \
    cursorpage.cpp \
    settingswindow.cpp \
    aboutwindow.cpp \
    animatedimage.cpp \
    blynkwindow.cpp \
    bluelightreducerslider.cpp

RESOURCES += \
    resources.qrc

FORMS += \
    dimmerwidget.ui \
    customslider.ui \
    customtooltip.ui \
    cursorpage.ui \
    screenbreakpage.ui \
    bluelightreducerpage.ui \
    cursorpage.ui \
    settingswindow.ui \
    aboutwindow.ui \
    blynkwindow.ui

