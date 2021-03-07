QT       += core gui
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# C++17 support
CONFIG += c++17

# Disable QDebug for release builds
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Enable as many compiler warnings as possible
CONFIG += warn_on

SOURCES += \
    batch_generator/batch_generator.cpp \
    common/text.cpp \
    jersey/design_item.cpp \
    jersey/font_item.cpp \
    jersey/font_server.cpp \
    jersey/generic_design_server.cpp \
    jersey/jersey.cpp \
    jersey/jersey_image_item.cpp \
    jersey/jersey_image_server.cpp \
    jersey/team_design_server.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/batch_generator_widget.cpp \
    ui/colour_widget.cpp \
    ui/preview_widget.cpp \
    ui/registry_settings_widget_server.cpp \
    ui/settings_widget.cpp

HEADERS += \
    batch_generator/batch_generator.h \
    common/dimensions.h \
    common/text.h \
    jersey/design_item.h \
    jersey/font_item.h \
    jersey/font_server.h \
    jersey/generic_design_server.h \
    jersey/jersey.h \
    jersey/jersey_image_item.h \
    jersey/jersey_image_server.h \
    jersey/team_design_server.h \
    mainwindow.h \
    ui/batch_generator_widget.h \
    ui/colour_widget.h \
    ui/preview_widget.h \
    ui/registry_settings_widget_server.h \
    ui/settings_widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#QXlsx library
Debug:LIBS += -L"$$_PRO_FILE_PWD_/qxlsx/lib" -lQXlsx_debug
Release:LIBS += -L"$$_PRO_FILE_PWD_/qxlsx/lib" -lQXlsx
INCLUDEPATH += $$_PRO_FILE_PWD_/qxlsx/include

RESOURCES += \
    EHM_Jersey_Generator.qrc

DISTFILES += \
    README.md \
    generic_jerseys.xlsx \
    images/layer_foreground/ana-foreground-2020.png \
    images/layer_foreground/ari-foreground-2020.png \
    images/layer_foreground/bos-foreground-2020.png \
    images/layer_foreground/buf-foreground-2020.png \
    images/layer_foreground/car-foreground-2020.png \
    images/layer_foreground/cgy-foreground-2020-01.png \
    images/layer_foreground/cgy-foreground-2020-02.png \
    images/layer_foreground/cgy-foreground-2020.png \
    images/layer_foreground/chi-foreground-2020.png \
    images/layer_foreground/clb-foreground-2020.png \
    images/layer_foreground/col-foreground-2020.png \
    images/layer_foreground/dal-foreground-2020.png \
    images/layer_foreground/det-foreground-2020.png \
    images/layer_foreground/edm-foreground-2020.png \
    images/layer_foreground/fla-foreground-2020.png \
    images/layer_foreground/lak-foreground-2020.png \
    images/layer_foreground/min-foreground-2020.png \
    images/layer_foreground/mtl-foreground-2020.png \
    images/layer_foreground/nas-foreground-2020.png \
    images/layer_foreground/njd-foreground-2020.png \
    images/layer_foreground/nyi-foreground-2020.png \
    images/layer_foreground/nyr-foreground-2020.png \
    images/layer_foreground/ott-foreground-2020.png \
    images/layer_foreground/phi-foreground-2020.png \
    images/layer_foreground/pit-foreground-2020.png \
    images/layer_foreground/sjs-foreground-2020.png \
    images/layer_foreground/stl-foreground-2020.png \
    images/layer_foreground/tbl-foreground-2020.png \
    images/layer_foreground/tor-foreground-2020.png \
    images/layer_foreground/van-foreground-2020.png \
    images/layer_foreground/veg-foreground-2020.png \
    images/layer_foreground/was-foreground-2020.png \
    images/layer_foreground/wpg-foreground-2020.png \
    images/layer_trim/ana-trim-2020.png \
    images/layer_trim/ari-trim-2020.png \
    images/layer_trim/bos-trim-2020.png \
    images/layer_trim/buf-trim-2020.png \
    images/layer_trim/car-trim-2020.png \
    images/layer_trim/cgy-trim-2020.png \
    images/layer_trim/chi-trim-2020.png \
    images/layer_trim/clb-trim-2020.png \
    images/layer_trim/col-trim-2020.png \
    images/layer_trim/dal-trim-2020.png \
    images/layer_trim/det-trim-2020.png \
    images/layer_trim/edm-trim-2020.png \
    images/layer_trim/fla-trim-2020.png \
    images/layer_trim/lak-trim-2020.png \
    images/layer_trim/min-trim-2020.png \
    images/layer_trim/mtl-trim-2020.png \
    images/layer_trim/nas-trim-2020.png \
    images/layer_trim/njd-trim-2020.png \
    images/layer_trim/nyi-trim-2020.png \
    images/layer_trim/nyr-trim-2020.png \
    images/layer_trim/ott-trim-2020.png \
    images/layer_trim/phi-trim-2020.png \
    images/layer_trim/pit-trim-2020.png \
    images/layer_trim/sjs-trim-2020-01.png \
    images/layer_trim/sjs-trim-2020-02.png \
    images/layer_trim/sjs-trim-2020.png \
    images/layer_trim/stl-trim-2020-01.png \
    images/layer_trim/stl-trim-2020-02.png \
    images/layer_trim/stl-trim-2020-03.png \
    images/layer_trim/stl-trim-2020.png \
    images/layer_trim/tbl-trim-2020.png \
    images/layer_trim/tor-trim-2020.png \
    images/layer_trim/van-trim-2020.png \
    images/layer_trim/veg-trim-2020-01.png \
    images/layer_trim/veg-trim-2020-02.png \
    images/layer_trim/was-trim-2020.png \
    images/layer_trim/wpg-trim-2020.png \
    qxlsx/lib/QXlsx.lib \
    qxlsx/lib/QXlsx_debug.lib \
    team_jerseys.xlsx
