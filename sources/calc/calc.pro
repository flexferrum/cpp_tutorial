TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    calculator.cpp \
    input_parser.cpp \
#    shared_pointer.cpp

HEADERS += \
    calculator.h \
    input_parser.h
