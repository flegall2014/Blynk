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
    customslider.h \
    dimmerwidget.h \
    customwindow.h \
    aboutwindow.h \
    animatedimage.h \
    blynkwindow.h \
    bluelightreducerslider.h \
    slider.h

SOURCES += \
    main.cpp \
    controller.cpp \
    blynk.cpp \
    imagewidget.cpp \
    parameters.cpp \
    utils.cpp \
    customslider.cpp \
    dimmerwidget.cpp \
    customwindow.cpp \
    aboutwindow.cpp \
    animatedimage.cpp \
    blynkwindow.cpp \
    bluelightreducerslider.cpp \
    slider.cpp

RESOURCES += \
    resources.qrc

FORMS += \
    dimmerwidget.ui \
    customslider.ui \
    aboutwindow.ui \
    blynkwindow.ui \
    slider.ui

