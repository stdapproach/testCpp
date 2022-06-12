#TEMPLATE = app
TEMPLATE  = lib
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

#DESTDIR = $$PWD/build/$$build_subdir
DESTDIR = ../build/$$build_subdir
#release: DESTDIR = build/release
#debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
#CONFIG(debug, debug|release) {
#    DESTDIR = ../build/debug
#} else {
#    DESTDIR = ../build/release
#}

#DEFINES += "DLL_EXPORT"

SOURCES += \
        c1.cpp \
        main.cpp \
        state.cpp

HEADERS += \
    h1.h \
    state.h
