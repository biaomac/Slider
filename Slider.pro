#-------------------------------------------------
#
# Project created by QtCreator 2013-06-01T16:25:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Slider
TEMPLATE = app


SOURCES += main.cpp\
        Widget.cpp \
    SliderGroove.cpp \
    SliderHandle.cpp \
    Slider.cpp

HEADERS  += Widget.hpp \
    SliderGroove.hpp \
    SliderHandle.hpp \
    Slider.hpp

FORMS    += Widget.ui
