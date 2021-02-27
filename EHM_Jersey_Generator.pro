QT       += core gui

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
    jersey/font_item.cpp \
    jersey/font_server.cpp \
    jersey/jersey.cpp \
    jersey/jersey_image_item.cpp \
    jersey/jersey_image_server.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/colour_widget.cpp \
    ui/preview_widget.cpp \
    ui/settings_widget.cpp

HEADERS += \
    dimensions.h \
    jersey/font_item.h \
    jersey/font_server.h \
    jersey/jersey.h \
    jersey/jersey_image_item.h \
    jersey/jersey_image_server.h \
    mainwindow.h \
    ui/colour_widget.h \
    ui/preview_widget.h \
    ui/settings_widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    EHM_Jersey_Generator.qrc

DISTFILES += \
    README.md \
    images/layer_foreground/ana-foreground.png \
    images/layer_foreground/ari-foreground.png \
    images/layer_foreground/bos-foreground.png \
    images/layer_foreground/buf-foreground.png \
    images/layer_foreground/car-foreground.png \
    images/layer_foreground/cgy-foreground-01.png \
    images/layer_foreground/cgy-foreground-02.png \
    images/layer_foreground/chi-foreground.png \
    images/layer_foreground/clb-foreground.png \
    images/layer_foreground/col-foreground.png \
    images/layer_foreground/dal-foreground.png \
    images/layer_foreground/det-foreground.png \
    images/layer_foreground/edm-foreground.png \
    images/layer_foreground/fla-foreground.png \
    images/layer_foreground/lak-foreground.png \
    images/layer_foreground/min-foreground.png \
    images/layer_foreground/mtl-foreground.png \
    images/layer_trim/ana-trim.png \
    images/layer_trim/ari-trim.png \
    images/layer_trim/bos-trim.png \
    images/layer_trim/buf-trim.png \
    images/layer_trim/car-trim.png \
    images/layer_trim/cgy-trim.png \
    images/layer_trim/chi-trim.png \
    images/layer_trim/clb-trim.png \
    images/layer_trim/col-trim.png \
    images/layer_trim/dal-trim.png \
    images/layer_trim/det-trim.png \
    images/layer_trim/edm-trim.png \
    images/layer_trim/fla-trim.png \
    images/layer_trim/lak-trim.png \
    images/layer_trim/min-trim.png \
    images/layer_trim/mtl-trim.png
