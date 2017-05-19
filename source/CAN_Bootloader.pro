#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T09:26:29
#
#-------------------------------------------------

QT       += core gui

TARGET = CAN_Bootloader
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:LIBS += -L$$PWD/../lib/ -lGinkgo_Driver

win32:LIBS += -L$$PWD/../lib/ -lUSB2XXX

#DEFINES += LANGUE_EN

RESOURCES += \
    image.qrc

RC_FILE = \
    ico.rc

FORMS += \
    mainwindow_ch.ui \
    mainwindow_en.ui \
    devaddrinputdialog.ui

OTHER_FILES += \
    ico.rc

HEADERS += \
    CANBootloader.h \
    ErrorType.h \
    mainwindow.h \
    ControlCAN.h \
    devaddrinputdialog.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    devaddrinputdialog.cpp
