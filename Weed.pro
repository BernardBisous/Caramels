QT       += core gui charts multimedia multimediawidgets serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Interface/actionwidget.cpp \
    Interface/devicelistwidget.cpp \
    Interface/drawerselector.cpp \
    Interface/emailnotifier.cpp \
    Interface/events.cpp \
    Interface/menubutton.cpp \
    Interface/namelabel.cpp \
    Interface/overview.cpp \
    Interface/plantationeditor.cpp \
    Interface/progressbar.cpp \
    Interface/progresswidget.cpp \
    Interface/roundedpicture.cpp \
    Interface/scrollarea.cpp \
    Interface/serialeditor.cpp \
    Interface/slider.cpp \
    Interface/switchcheckbox.cpp \
    Interface/toolbutton.cpp \
    chemicalmanager.cpp \
    configeditor.cpp \
    configoverview.cpp \
    configprogress.cpp \
    configtop.cpp \
    consolewidget.cpp \
    deviceeditor.cpp \
    deviceplot.cpp \
    eventmanager.cpp \
    growconfig.cpp \
    growingtent.cpp \
    hardware/analogsensor.cpp \
    hardware/booleansensor.cpp \
    hardware/chemicalinjector.cpp \
    hardware/co2manager.cpp \
    hardware/device.cpp \
    hardware/distancesensor.cpp \
    hardware/hardwareunit.cpp \
    hardware/lights.cpp \
    hardware/phmanager.cpp \
    hardware/pump.cpp \
    hardware/serialtent.cpp \
    hardware/temperaturemanager.cpp \
    hardware/tolleveler.cpp \
    hardware/waterlevelmanager.cpp \
    hardware/webcam.cpp \
    hardwareoverview.cpp \
    integralplot.cpp \
    main.cpp \
    parameter.cpp \
    parametereditor.cpp \
    parameterlistwidget.cpp \
    parameterplot.cpp \
    parametervalueeditor.cpp \
    plantation.cpp \
    regulatorwidget.cpp \
    resultplot.cpp \
    tent.cpp \
    tenteditor.cpp \
    tentlistwidget.cpp \
    uniteditor.cpp \
    unitoverview.cpp \
    unitoverviews.cpp \
    unitplot.cpp \
    webcamwidget.cpp

HEADERS += \
    Interface/actionwidget.h \
    Interface/devicelistwidget.h \
    Interface/drawerselector.h \
    Interface/emailnotifier.h \
    Interface/events.h \
    Interface/menubutton.h \
    Interface/namelabel.h \
    Interface/overview.h \
    Interface/plantationeditor.h \
    Interface/progressbar.h \
    Interface/progresswidget.h \
    Interface/roundedpicture.h \
    Interface/scrollarea.h \
    Interface/serialeditor.h \
    Interface/slider.h \
    Interface/switchcheckbox.h \
    Interface/toolbutton.h \
    chemicalmanager.h \
    configeditor.h \
    configoverview.h \
    configprogress.h \
    configtop.h \
    consolewidget.h \
    deviceeditor.h \
    deviceplot.h \
    eventmanager.h \
    growconfig.h \
    growingtent.h \
    hardware/Pinout.h \
    hardware/analogsensor.h \
    hardware/booleansensor.h \
    hardware/chemicalinjector.h \
    hardware/co2manager.h \
    hardware/device.h \
    hardware/distancesensor.h \
    hardware/hardwareunit.h \
    hardware/lights.h \
    hardware/phmanager.h \
    hardware/pump.h \
    hardware/serialtent.h \
    hardware/temperaturemanager.h \
    hardware/tolleveler.h \
    hardware/waterlevelmanager.h \
    hardware/webcam.h \
    hardwareoverview.h \
    integralplot.h \
    parameter.h \
    parametereditor.h \
    parameterlistwidget.h \
    parameterplot.h \
    parametervalueeditor.h \
    plantation.h \
    regulatorwidget.h \
    resultplot.h \
    tent.h \
    tenteditor.h \
    tentlistwidget.h \
    uniteditor.h \
    unitoverview.h \
    unitoverviews.h \
    unitplot.h \
    webcamwidget.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Config.qrc \
    icons.qrc

DISTFILES += \
    arduinoTent/arduinoTent.ino \

