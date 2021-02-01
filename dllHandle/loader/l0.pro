TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -ldl

DESTDIR = ../build/$$build_subdir

SOURCES += \
        main.cpp

HEADERS += \
    dllHandler.h
