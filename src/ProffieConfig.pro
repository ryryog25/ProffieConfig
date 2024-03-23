TEMPLATE = app
CONFIG += c++17
CONFIG -= qt

QMAKE_CXXFLAGS += $$system(wx-config --cxxflags)
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14

VERSION = 2.0.0
DEFINES += VERSION=\\\"$$VERSION\\\"
win32: DEFINES += wxMessageDialog=wxGenericMessageDialog
win32: DEFINES += wxProgressDialog=wxGenericProgressDialog
win32: DEFINES += wxAboutBox=wxGenericAboutBox

linux-g++ {
        DEFINES += PROPPATH=\\\"resources/props/\\\"
        DEFINES += CONFIGPATH=\\\"resources/ProffieOS/config/\\\"
}

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
        appcore/state.cpp \
        config/config.cpp \
        config/defaults.cpp \
        config/settings.cpp \
        main.cpp \
        log/logger.cpp \
        pconf/pconf.cpp \
        prop/propfile.cpp \
        styles/elements/colors.cpp \
        styles/elements/effects.cpp \
    styles/elements/lockuptype.cpp \
        styles/generator.cpp \
        styles/style.cpp \
        test/styleedit.cpp \
        ui/combobox.cpp \
        ui/numeric.cpp \
        ui/numericdec.cpp \
        ui/text.cpp \
        ui/toggle.cpp \
        ui/selection.cpp \

HEADERS += \
        appcore/state.h \
        config/config.h \
        config/defaults.h \
        config/settings.h \
        log/logger.h \
        pconf/pconf.h \
        prop/propfile.h \
        styles/base.h \
    styles/elements/builtin.h \
        styles/elements/colors.h \
        styles/elements/effects.h \
        styles/elements/functions.h \
    styles/elements/lockuptype.h \
        styles/elements/wrappers.h \
        styles/generator.h \
        styles/style.h \
        test/styleedit.h \
        ui/combobox.h \
        ui/numeric.h \
        ui/numericdec.h \
        ui/text.h \
        ui/toggle.h \
        ui/selection.h \
