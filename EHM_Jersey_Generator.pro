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
    jersey.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/colour_widget.cpp \
    ui/preview_widget.cpp

HEADERS += \
    dimensions.h \
    jersey.h \
    mainwindow.h \
    ui/colour_widget.h \
    ui/preview_widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    EHM_Jersey_Generator.qrc
