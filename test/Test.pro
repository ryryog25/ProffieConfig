TEMPLATE = app
CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console
CONFIG += c++14

INCLUDEPATH += ../src
SOURCES = tst_tyles.cpp

CATCH2_MAIN=1

include(pc-files.pri)
include(catch-common.pri)
