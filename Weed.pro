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
    config/archive.cpp \
    config/events.cpp \
    hardware/devicetester.cpp \
    hardware/generalmanager.cpp \
    Interface/menubutton.cpp \
    Interface/namelabel.cpp \
    Interface/overview.cpp \
    Interface/progressbar.cpp \
    Interface/progresswidget.cpp \
    Interface/roundedpicture.cpp \
    Interface/scrollarea.cpp \
    Interface/serialeditor.cpp \
    Interface/slider.cpp \
    Interface/switchcheckbox.cpp \
    Interface/toolbutton.cpp \
    SMTP/emailaddress.cpp \
    SMTP/mimebase64encoder.cpp \
    SMTP/mimebase64formatter.cpp \
    SMTP/mimebytearrayattachment.cpp \
    SMTP/mimecontentencoder.cpp \
    SMTP/mimecontentformatter.cpp \
    SMTP/mimefile.cpp \
    SMTP/mimehtml.cpp \
    SMTP/mimeinlinefile.cpp \
    SMTP/mimemessage.cpp \
    SMTP/mimemultipart.cpp \
    SMTP/mimepart.cpp \
    SMTP/mimeqpencoder.cpp \
    SMTP/mimeqpformatter.cpp \
    SMTP/mimetext.cpp \
    SMTP/quotedprintable.cpp \
    SMTP/smtpclient.cpp \
    widgets/archivewidget.cpp \
    widgets/configeditor.cpp \
    widgets/configoverview.cpp \
    widgets/configprogress.cpp \
    widgets/consolewidget.cpp \
    widgets/deviceeditor.cpp \
    widgets/deviceplot.cpp \
    hardware/devicestate.cpp \
    config/eventmanager.cpp \
    config/growconfig.cpp \
    growingtent.cpp \
    hardware/analogsensor.cpp \
    hardware/chemicalmanager.cpp \
    hardware/booleansensor.cpp \
    hardware/chemicalinjector.cpp \
    hardware/co2manager.cpp \
    hardware/device.cpp \
    hardware/distancesensor.cpp \
    hardware/hardwareunit.cpp \
    hardware/lights.cpp \
    hardware/phmanager.cpp \
    hardware/pump.cpp \
    hardware/regulatingtimer.cpp \
    hardware/serialtent.cpp \
    hardware/temperaturemanager.cpp \
    hardware/tolleveler.cpp \
    hardware/waterlevelmanager.cpp \
    hardware/webcam.cpp \
    widgets/eventwizzard.cpp \
    widgets/hardwareoverview.cpp \
    widgets/integralplot.cpp \
    main.cpp \
    config/parameter.cpp \
    widgets/parametereditor.cpp \
    widgets/parameterlistwidget.cpp \
    widgets/parameterplot.cpp \
    widgets/parametervalueeditor.cpp \
    widgets/regulatorwidget.cpp \
    hardware/statenotifier.cpp \
    widgets/setupwizzard.cpp \
    widgets/statewidget.cpp \
    hardware/tent.cpp \
    widgets/tenteditor.cpp \
    widgets/uniteditor.cpp \
    widgets/unitoverview.cpp \
    widgets/unitplot.cpp \
    widgets/webcamoverview.cpp \
    widgets/webcamwidget.cpp \
    widgets/wizzard.cpp

HEADERS += \
    Interface/actionwidget.h \
    Interface/devicelistwidget.h \
    Interface/drawerselector.h \
    Interface/emailnotifier.h \
    Interface/menubutton.h \
    Interface/namelabel.h \
    Interface/overview.h \
    Interface/progressbar.h \
    Interface/progresswidget.h \
    Interface/roundedpicture.h \
    Interface/scrollarea.h \
    Interface/serialeditor.h \
    Interface/slider.h \
    Interface/switchcheckbox.h \
    Interface/toolbutton.h \
    SMTP/SmtpMime \
    SMTP/emailaddress.h \
    SMTP/mimebase64encoder.h \
    SMTP/mimebase64formatter.h \
    SMTP/mimebytearrayattachment.h \
    SMTP/mimecontentencoder.h \
    SMTP/mimecontentformatter.h \
    SMTP/mimefile.h \
    SMTP/mimehtml.h \
    SMTP/mimeinlinefile.h \
    SMTP/mimemessage.h \
    SMTP/mimemultipart.h \
    SMTP/mimepart.h \
    SMTP/mimeqpencoder.h \
    SMTP/mimeqpformatter.h \
    SMTP/mimetext.h \
    SMTP/quotedprintable.h \
    SMTP/smtpclient.h \
    SMTP/smtpmime_global.h \
    config/archive.h \
    config/events.h \
    hardware/devicetester.h \
    widgets/archivewidget.h \
    widgets/configeditor.h \
    widgets/configoverview.h \
    widgets/configprogress.h \
    widgets/consolewidget.h \
    widgets/deviceeditor.h \
    widgets/deviceplot.h \
    hardware/devicestate.h \
    config/eventmanager.h \
    config/growconfig.h \
    growingtent.h \
    hardware/Pinout.h \
    hardware/generalmanager.h \
    hardware/chemicalmanager.h \
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
    hardware/regulatingtimer.h \
    hardware/serialtent.h \
    hardware/temperaturemanager.h \
    hardware/tolleveler.h \
    hardware/waterlevelmanager.h \
    hardware/webcam.h \
    widgets/eventwizzard.h \
    widgets/hardwareoverview.h \
    widgets/integralplot.h \
    config/parameter.h \
    widgets/parametereditor.h \
    widgets/parameterlistwidget.h \
    widgets/parameterplot.h \
    widgets/parametervalueeditor.h \
    widgets/regulatorwidget.h \
    hardware/statenotifier.h \
    widgets/setupwizzard.h \
    widgets/statewidget.h \
    hardware/tent.h \
    widgets/tenteditor.h \
    widgets/uniteditor.h \
    widgets/unitoverview.h \
    widgets/unitplot.h \
    widgets/webcamoverview.h \
    widgets/webcamwidget.h \
    widgets/wizzard.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Config.qrc \
    Images.qrc \
    emails.qrc \
    icons.qrc

DISTFILES += \
    arduinoTent/arduinoTent.ino \

