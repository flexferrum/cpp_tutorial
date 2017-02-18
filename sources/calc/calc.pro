TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    input_parser.cpp \
    processor.cpp \
    Calculator.cpp

HEADERS += \
    input_parser.h \
    processor.h \
    command_base.h \
    commands_visitor_base.h \
    Calculator.h \
    math_operations.h
