#!qtcreator
QT += core network
QT -= gui

CONFIG += c++14

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

PRECOMPILED_HEADER = chai_stable.h
HEADERS += chai_stable.h
SOURCES += chai.cpp

DEFINES += CHAISCRIPT_NO_THREADS=1 CHAISCRIPT_NO_THREADS_WARNING=1
INCLUDEPATH += jx9-1007002 duktape-2.2.0/src-noline ChaiScript-6.0.0\include
