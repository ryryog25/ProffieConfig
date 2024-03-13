TEMPLATE = app
CONFIG += c++17
CONFIG -= qt

QMAKE_CXXFLAGS += $$system(wx-config --cxxflags)
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14

VERSION = 1.6.6
DEFINES += VERSION=\\\"$$VERSION\\\"
win32: DEFINES += wxMessageDialog=wxGenericMessageDialog
win32: DEFINES += wxProgressDialog=wxGenericProgressDialog
win32: DEFINES += wxAboutBox=wxGenericAboutBox

# macOS resource handling
ICON = ../resources/icons/icon.icns
RESOURCE.files += ../resources/macOS/arduino-cli
RESOURCE.files += ../resources/ProffieOS
RESOURCE.files += ../resources/StyleEditor
RESOURCE.files += ../resources/props
RESOURCE.path = Contents/Resources
QMAKE_BUNDLE_DATA += RESOURCE

# Windows resource handling
RC_ICONS = ../resources/icons/icon.ico
RC_DEFINES += VERSION=\\\"$$VERSION\\0\\\" WIN_VERSION=$$replace(VERSION, "\.", ","),0
RC_INCLUDEPATH = /usr/x86_64-w64-mingw32/include/wx-3.3/
RC_FILE += ./ProffieConfig_resource.rc

LIBS += $$system(wx-config --libs all)

SOURCES += \
        main.cpp \
        pconf.cpp

HEADERS += \
    pconf.h
