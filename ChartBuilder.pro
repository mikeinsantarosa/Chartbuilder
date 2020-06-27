#-------------------------------------------------
#
# Project created by QtCreator 2019-03-27T15:55:05
#
#-------------------------------------------------

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.1.2
DEFINES += APP_VERSION=$$VERSION

TARGET = ChartBuilder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    datapoint.cpp \
        main.cpp \
        htcdataselector.cpp \
    htcchartdatafile.cpp \
    htcchartdatamangler.cpp \
    htcchartfolder.cpp \
    chartproperties.cpp \
    htcchart.cpp \
    htcchartdataset.cpp \
    htcdatadialog.cpp \
    about.cpp \
    htcsettings.cpp \
    positioneditor.cpp



HEADERS += \
    datapoint.h \
        htcdataselector.h \
    htcchartdatafile.h \
    htcchartdatamangler.h \
    htcchartfolder.h \
    htcchartdataset.h \
    chartproperties.h \
    htcchart.h \
    htcdatadialog.h \
    about.h \
    positioneditor.h \
    version.h \
    htcsettings.h



FORMS += \
        htcdataselector.ui \
    chartproperties.ui \
    htcchart.ui \
    htcdatadialog.ui \
    about.ui \
    htcsettings.ui \
    positioneditor.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

SUBDIRS += \
    ChartBuilder.pro
