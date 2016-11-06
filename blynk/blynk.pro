#-------------------------------------------------
#
# Project created by QtCreator 2016-10-23T09:16:18
#
#-------------------------------------------------

QT       += core gui xml widgets

TEMPLATE = app

INCLUDEPATH += $$PWD/../utils ../singleapplication ../gammaramp

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../bin/ -lutilsd -lsingleapplicationd -lgammarampd
    TARGET = blynkd
} else {
    LIBS += -L$$PWD/../bin/ -lutils -lsingleapplication -lgammaramp
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
    preferencedialog.h \
    imagewidget.h \
    parameters.h \
    utils.h \
    bluelightreducerwidget.h \
    customslider.h \
    animatedcursor.h \
    dimmerwidget.h \
    customtooltip.h

SOURCES += \
    main.cpp \
    controller.cpp \
    blynk.cpp \
    preferencedialog.cpp \
    imagewidget.cpp \
    parameters.cpp \
    utils.cpp \
    bluelightreducerwidget.cpp \
    customslider.cpp \
    animatedcursor.cpp \
    dimmerwidget.cpp \
    customtooltip.cpp

RESOURCES += \
    resources.qrc

FORMS += \
    preferencedialog.ui \
    dimmerwidget.ui \
    customslider.ui \
    customtooltip.ui

