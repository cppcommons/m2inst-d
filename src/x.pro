#!qtcreator
QT += core
QT -= gui

CONFIG += c++11

TARGET = x
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += x.h

SOURCES += x.c

DEFINES += TEST_MAIN=1

LIBS += -lwinhttp
