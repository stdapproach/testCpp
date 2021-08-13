TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        processor/Processor.cpp

HEADERS += \
    model/IModel.h \
    model/M0.h \
    processor/ISandBox.h \
    processor/Processor.h
