#-------------------------------------------------
#
# Project created by QtCreator 2016-10-31T14:16:40
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so  \
        /usr/local/lib/libopencv_objdetect.so

SOURCES += main.cpp \
    cvText.cpp \
    ReArrangement.cpp \
    closestPoint.cpp

HEADERS += \
    header.h

OTHER_FILES +=
