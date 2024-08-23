QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Interface/actionwidget.cpp \
    Interface/drawerselector.cpp \
    Interface/menubutton.cpp \
    Interface/namelabel.cpp \
    Interface/scrollarea.cpp \
    Interface/slider.cpp \
    Interface/toolbutton.cpp \
    configeditor.cpp \
    deviceeditor.cpp \
    deviceplot.cpp \
    growconfig.cpp \
    growingtent.cpp \
    hardware/device.cpp \
    hardware/lights.cpp \
    hardware/pump.cpp \
    main.cpp \
    parameter.cpp \
    parametereditor.cpp \
    parameterlistwidget.cpp \
    parameterplot.cpp \
    tent.cpp \
    tenteditor.cpp \
    tentlistwidget.cpp

HEADERS += \
    Interface/actionwidget.h \
    Interface/drawerselector.h \
    Interface/menubutton.h \
    Interface/namelabel.h \
    Interface/scrollarea.h \
    Interface/slider.h \
    Interface/toolbutton.h \
    configeditor.h \
    deviceeditor.h \
    deviceplot.h \
    growconfig.h \
    growingtent.h \
    hardware/Pinout.h \
    hardware/device.h \
    hardware/lights.h \
    hardware/pump.h \
    parameter.h \
    parametereditor.h \
    parameterlistwidget.h \
    parameterplot.h \
    tent.h \
    tenteditor.h \
    tentlistwidget.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
