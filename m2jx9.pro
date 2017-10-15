#!qtcreator
QT += core network
QT -= gui

CONFIG += c++11

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += m2boot.cpp \
    jx9-1007002/jx9.c \
    duktape-2.2.0/src-noline/duktape.c

HEADERS += \
    jx9-1007002/jx9.h \
    duktape-2.2.0/src-noline/duk_config.h \
    duktape-2.2.0/src-noline/duktape.h

INCLUDEPATH += jx9-1007002 duktape-2.2.0/src-noline
